//
//  MasterBuilder.cpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <fstream>
#include "MasterBuilder.hpp"
#include "SQLiteSensorValueSerializer.hpp"
#include "HomePlanRestApiHandler.hpp"
#include "PhysicalSensorRestApiHandler.hpp"
#include "MeasurementsRestApiHandler.hpp"
#include "JSONHelper.hpp"
#include "TemperatureIdentifier.hpp"
#include "LogHelper.hpp"

static const string KEY_HEATING_CONFIG_FILE = "heatingPlan";
static const string KEY_HOME_PLAN_CONFIG_FILE = "homePlan";
static const string KEY_SENSORS_CONFIG_FILE = "sensors";
static const string KEY_STORAGE_DB_FILE = "dbFile";

static const string KEY_WEB_SERVER_CONFIG = "webServer";
static const string KEY_WEB_PORT = "port";

static const string KEY_LOGGING = "logging";
static const string KEY_LOGGING_LEVEL = "level";
static const string KEY_LOGGING_CONSOLE = "console";
static const string KEY_LOGGING_FILE = "file";
static const string KEY_LOGGING_FORMAT = "format";

MasterBuilder::MasterBuilder(string const& configFile)
: wasBuilded(false) {
  
  std::ifstream inputFileStream(configFile);
  if (inputFileStream.good() == false) {
    throw invalid_argument("Can't open confguration:" + configFile);
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  
  masterConfig = json::parse(buffer.str());
  
  vector<string> expected = {KEY_HEATING_CONFIG_FILE, KEY_HOME_PLAN_CONFIG_FILE,
    KEY_SENSORS_CONFIG_FILE, KEY_STORAGE_DB_FILE};
  string missing;
  if (checkIfKeysExists(masterConfig, expected, &missing) == false){
    fprintf(stderr, "Config is wrong. Lack of mandatory node '%s' in file: %s \n",
            missing.c_str(), configFile.c_str());
    throw invalid_argument("Wrong format of config file.");
  }
}

void MasterBuilder::build() {
  wasBuilded = true;
  buildLoggers();
  buildLogic();
  buildHttpServer();
}

void MasterBuilder::buildLoggers() {
  int logLevel = 4;
  bool logToConsole = false;
  string logFile = "logs";
  string customFormat = "";
  
  if (checkIfKeysExists(masterConfig, {KEY_LOGGING})) {
    //Global log level
    json loggerConf = masterConfig[KEY_LOGGING];
    long tmp = getOptionalJSONLong(loggerConf, KEY_LOGGING_LEVEL);
    if (tmp > -1) {
      logLevel = static_cast<int>(tmp);
    }
    
    getOptionalJSONBool(loggerConf, KEY_LOGGING_CONSOLE, logToConsole);
    
    shared_ptr<string> tmpS = getOptionalJSONString(loggerConf, KEY_LOGGING_FILE);
    if (tmpS != nullptr) {
      logFile = *tmpS;
    }
    
    tmpS = getOptionalJSONString(loggerConf, KEY_LOGGING_FORMAT);
    if (tmpS != nullptr) {
      customFormat = *tmpS;
    }
  }
  
  createLoggers(logLevel, logToConsole, logFile, customFormat);
  
  //override loggers if extra configs found
  for(auto iter = loggerNames.begin(); iter != loggerNames.end(); iter ++) {
    if (checkIfKeysExists(masterConfig, {*iter}) == false) {
      continue;
    }
    json subConf = masterConfig[*iter];
    
    long tmp = getOptionalJSONLong(subConf, KEY_LOGGING_LEVEL);
    if (tmp > -1) {
      int level = static_cast<int>(tmp);
      overrideLogger(*iter, &level, nullptr);
    }
    
    shared_ptr<string> tmpS = getOptionalJSONString(subConf, KEY_LOGGING_FORMAT);
    if (tmpS != nullptr) {
      overrideLogger(*iter, nullptr, &(*tmpS));
    }
  }
}

shared_ptr<Logic> MasterBuilder::getLogic() {
  if (wasBuilded == false) {
    build();
  }
  return logic;
}

shared_ptr<HttpServer> MasterBuilder::getHttpServer() {
  if (wasBuilded == false) {
    build();
  }
  return httpServer;
}

void MasterBuilder::buildStorage() {
  string dbFile = masterConfig[KEY_STORAGE_DB_FILE];
  storage = make_shared<SQLiteStorage>(dbFile);
  storage->registerSerializer(SensorValue(), new SQLiteSensorValueSerializer());
  storage->open();
}

void MasterBuilder::buildSensors() {
  sensorNetManager = make_shared<SensorNetManager>();
  
  string configFile = masterConfig[KEY_SENSORS_CONFIG_FILE];
  std::ifstream inputFileStream(configFile);
  if (inputFileStream.good() == false) {
    throw invalid_argument("Can't open config:" + configFile);
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  PhysicalSensorList sensors = physicalSensorsFromJSON(buffer.str());
  sensorNetManager->setSensorsList(sensors);
}

void MasterBuilder::buildHeatingPlan() {
  
  string configFile = masterConfig[KEY_HEATING_CONFIG_FILE];
  std::ifstream inputFileStream( configFile );
  if (inputFileStream.good() == false) {
    throw invalid_argument("Can't open config:" + configFile);
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  
  json inJson = json::parse(buffer.str());
  
  vector<string> expected = {"temperatures", "heatingPlans", "roomsHeating"};
  string missing;
  if (checkIfKeysExists(inJson, expected, &missing) == false){
    fprintf(stderr, "Config is wrong. Lack of mandatory node '%s' in file: %s \n",
            missing.c_str(), configFile.c_str());
    throw invalid_argument("Wrong format of config file.");
  }
  
  parseConfigTemperatures(inJson["temperatures"]);
  parseHetingPlans(inJson["heatingPlans"]);
  parseRoomHeating(inJson["roomsHeating"]);
}


void MasterBuilder::parseHetingPlans(json const& definition) {
  heatingPlans = make_shared<ScheduleMap>();
  
  for(auto iter = definition.begin(); iter != definition.end(); iter++) {
    string planName = iter.key();
    json const& data = iter.value();
    shared_ptr<Schedule> schedule = make_shared<Schedule>(data, temperatures);
    (*heatingPlans)[planName] = schedule;
  }
}

void MasterBuilder::parseConfigTemperatures(json const& definition) {
  temperatures = make_shared<TemperatureIdentifierVector>();
  if (definition.is_array() == false) {
    fprintf(stderr, "Expected array here! \n%s\n", definition.dump().c_str());
    throw invalid_argument("Wrong format of 'temperatures' node.");
  }
  
  for(auto iter = definition.begin(); iter != definition.end(); iter ++) {
    if (checkIfKeysExists(*iter, {"id", "temperature"}) == false){
      fprintf(stderr, "Config, is wrong, expected nodes 'temperature', 'id' in line: %s \n",
              (*iter).dump().c_str());
      throw invalid_argument("Wrong format of config file.");
    }
    
    string name = (*iter)["id"];
    double temp = (*iter)["temperature"];
    temperatures->push_back( shared_ptr<TemperatureIdentifier>( new TemperatureIdentifier(name, temp) ) );
  }
}

void MasterBuilder::parseRoomHeating(json const& definition) {
  if (definition.is_array() == false) {
    fprintf(stderr, "Expected array here! \n%s\n", definition.dump().c_str());
    throw invalid_argument("Wrong format of 'roomsHeating' node.");
  }
  
  shared_ptr<ScheduleMap> roomHeatingPlan = logic->getRoomHeatingPlan();
  
  for(auto iter = definition.begin(); iter != definition.end(); iter ++) {
    if (checkIfKeysExists(*iter, {"room", "plan"}) == false){
      fprintf(stderr, "Config, is wrong, expected nodes 'room', 'plan' in line: %s \n",
              (*iter).dump().c_str());
      throw invalid_argument("Wrong format of config file.");
    }
    
    string room = (*iter)["room"];
    string plan = (*iter)["plan"];
    
    if (heatingPlans->find(plan) == heatingPlans->end()) {
      fprintf(stderr, "Config, is wrong, expected hetaing plan named %s in line: %s \n",
              plan.c_str(), (*iter).dump().c_str());
      throw invalid_argument("Undefined plan.");
    }
    
    shared_ptr<Schedule> schedule = (*heatingPlans)[plan];
    (*roomHeatingPlan)[room] = schedule;
  }
}

void MasterBuilder::buildLogic() {
  buildStorage();
  buildSensors();
  
  logic = make_shared<Logic>(storage, sensorNetManager);
  
  buildHeatingPlan();
}

void MasterBuilder::buildHttpServer() {
  int port = masterConfig[KEY_WEB_SERVER_CONFIG][KEY_WEB_PORT];
  httpServer = make_shared<HttpServer>(storage, port);
  httpServer->registerHandler(std::make_shared<HomePlanRestApiHandler>());
  httpServer->registerHandler(std::make_shared<PhysicalSensorRestApiHandler>(logic));
  httpServer->registerHandler(std::make_shared<MeasurementsRestApiHandler>(logic));
}


