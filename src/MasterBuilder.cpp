//
//  MasterBuilder.cpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <fstream>
#include "MasterBuilder.hpp"
#include "storage/sqlite/SQLiteSensorValueSerializer.hpp"
#include "http/HomePlanRestApiHandler.hpp"
#include "http/PhysicalSensorRestApiHandler.hpp"
#include "http/MeasurementsRestApiHandler.hpp"
#include "misc/JSONHelper.hpp"
#include "logic/TemperatureIdentifier.hpp"
#include "misc/LogHelper.hpp"
#include "logic/rules/Rules.hpp"
#include "SharedStatesConsts.h"

static const string KEY_HEATING_CONFIG_FILE = "heatingPlan";
static const string KEY_HOME_PLAN_CONFIG_FILE = "homePlan";
static const string KEY_SENSORS_CONFIG_FILE = "sensors";
static const string KEY_RELAYS_CONFIG_FILE = "relays";
static const string KEY_STORAGE_DB_FILE = "dbFile";
static const string KEY_USE_DB = "useDb";

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
  
  spdlog::get(MISC_LOGGER_NAME)->info("Loggers created");
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
  bool useDb = true;
  getOptionalJSONBool(masterConfig, KEY_USE_DB, useDb);
  if (useDb == false) {
    spdlog::get(MISC_LOGGER_NAME)->info("Usage of DB is disabled.");
    return;
  }
  string dbFile = masterConfig[KEY_STORAGE_DB_FILE];
  spdlog::get(MISC_LOGGER_NAME)->info("Building SQLite storage...");
  spdlog::get(MISC_LOGGER_NAME)->info("  DB File:{}", dbFile);
  storage = make_shared<SQLiteStorage>(dbFile);
  storage->registerSerializer(SensorValue(), new SQLiteSensorValueSerializer());
  storage->open();
  spdlog::get(MISC_LOGGER_NAME)->info("Building SQLite storage DONE.");
}

void MasterBuilder::buildSensors() {
  sensorNetManager = make_shared<SensorNetManager>();
  
  string configFile = masterConfig[KEY_SENSORS_CONFIG_FILE];
  sensorNetManager->setSensorsConfigFile(configFile);
  spdlog::get(MISC_LOGGER_NAME)->info("Building Sensors Net...");
  spdlog::get(MISC_LOGGER_NAME)->info("  Config:{}", configFile);
  std::ifstream inputFileStream(configFile);
  if (inputFileStream.good() == false) {
    throw invalid_argument("Can't open config:" + configFile);
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  PhysicalSensorList sensors = physicalSensorsFromJSON(buffer.str());
  sensorNetManager->setSensorsList(sensors);
  spdlog::get(MISC_LOGGER_NAME)->info("  Sensors count:{}", sensors->size());
  spdlog::get(MISC_LOGGER_NAME)->info("Building Sensors Net DONE.");
}

void MasterBuilder::buildRelays() {
  relaysStatesMachine = make_shared<RelaysStatesMachine>(spdlog::get(LOGIC_LOGGER_NAME));

  string configFile = masterConfig[KEY_RELAYS_CONFIG_FILE];
  relaysStatesMachine->setConfigFile(configFile);
  spdlog::get(MISC_LOGGER_NAME)->info("Building relays Net...");
  spdlog::get(MISC_LOGGER_NAME)->info("  Config:{}", configFile);

  int count = 0;
  std::ifstream inputFileStream(configFile);
  if (inputFileStream.good() == false) {
    throw invalid_argument("Can't open config:" + configFile);
  }

  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  json inJson = json::parse(buffer);

  if (inJson.is_array() == false) {
    throw invalid_argument("Expected JSON array in:" + configFile);
  }
  for (auto iter = inJson.begin(); iter != inJson.end(); iter++) {
    count++;
    buildSingleRelay(*iter);
  }

  spdlog::get(MISC_LOGGER_NAME)->info("  Relays count:{}", count);
  spdlog::get(MISC_LOGGER_NAME)->info("Building Relays Net DONE.");
}

void MasterBuilder::buildSingleRelay(json const& data) {
  long id = getOptionalJSONLong(data, "id");
  if (id < 0) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'id' in {}", data.dump());
    return;
  }

  long sensorId = getOptionalJSONLong(data, "sensorId");
  if (sensorId < 0) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'sensorId' in {}", data.dump());
    return;
  }

  long sensorIndex = getOptionalJSONLong(data, "sensorIndex");
  if (sensorIndex < 0) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'sensorIndex' in {}", data.dump());
    return;
  }

  bool defaultState;
  getOptionalJSONBool(data, "defaultState", defaultState);

  shared_ptr<string> name = getOptionalJSONString(data, "name");
  if (name == nullptr) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'name' in {}", data.dump());
    return;
  }

  shared_ptr<PhysicalSensor> sensor = sensorNetManager->getSensorById(sensorId);
  if (sensor == nullptr) {
    spdlog::get(MISC_LOGGER_NAME)->error("Can't find physical sensor with id:{}, error at:{}", sensorId, data.dump());
    return;
  }

  relaysStatesMachine->addRelay(id, *name, sensor, sensorIndex, defaultState);
}

void MasterBuilder::buildHeatingPlan() {
  
  string configFile = masterConfig[KEY_HEATING_CONFIG_FILE];
  spdlog::get(MISC_LOGGER_NAME)->info("Building heating plan...");
  spdlog::get(MISC_LOGGER_NAME)->info("  Config file:{}", configFile);
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
    spdlog::get(MISC_LOGGER_NAME)->error("  Config is wrong. Lack of mandatory node '{}'", missing);
    throw invalid_argument("Wrong format of config file.");
  }
  
  parseConfigTemperatures(inJson["temperatures"]);
  parseHetingPlans(inJson["heatingPlans"]);
  parseRoomHeating(inJson["roomsHeating"]);
  spdlog::get(MISC_LOGGER_NAME)->info("Building heating plan DONE.");
}


void MasterBuilder::parseHetingPlans(json const& definition) {
  heatingPlans = make_shared<ScheduleMap>();
  
  for(auto iter = definition.begin(); iter != definition.end(); iter++) {
    string planName = iter.key();
    json const& data = iter.value();
    shared_ptr<Schedule> schedule = make_shared<Schedule>(data, temperatures);
    (*heatingPlans)[planName] = schedule;
  }
  spdlog::get(MISC_LOGGER_NAME)->info("  Plans:{}", definition.size());
}

void MasterBuilder::parseConfigTemperatures(json const& definition) {
  temperatures = make_shared<TemperatureIdentifierVector>();
  if (definition.is_array() == false) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Expected array here:{}", definition.dump());
    throw invalid_argument("Wrong format of 'temperatures' node.");
  }
  
  for(auto iter = definition.begin(); iter != definition.end(); iter ++) {
    if (checkIfKeysExists(*iter, {"id", "temperature"}) == false){
      spdlog::get(MISC_LOGGER_NAME)->error("  Config, is wrong, expected nodes 'temperature', 'id' in line:{}",
              (*iter).dump());
      
      throw invalid_argument("Wrong format of config file.");
    }
    
    string name = (*iter)["id"];
    double temp = (*iter)["temperature"];
    temperatures->push_back( shared_ptr<TemperatureIdentifier>( new TemperatureIdentifier(name, temp) ) );
  }
  spdlog::get(MISC_LOGGER_NAME)->info("  Temperatures ids:{}", definition.size());
}

void MasterBuilder::parseRoomHeating(json const& definition) {
  if (definition.is_array() == false) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Expected array here:{}", definition.dump());
    throw invalid_argument("Wrong format of 'roomsHeating' node.");
  }
  
  shared_ptr<ScheduleMap> roomHeatingPlan = logic->getRoomHeatingPlan();
  
  for(auto iter = definition.begin(); iter != definition.end(); iter ++) {
    if (checkIfKeysExists(*iter, {"room", "plan"}) == false){
      spdlog::get(MISC_LOGGER_NAME)->error("  Config, is wrong, expected nodes 'room', 'plan' in line:{}",
              (*iter).dump());
      throw invalid_argument("Wrong format of config file.");
    }
    
    string room = (*iter)["room"];
    string plan = (*iter)["plan"];
    
    if (heatingPlans->find(plan) == heatingPlans->end()) {
      spdlog::get(MISC_LOGGER_NAME)->error("  Config, is wrong, expected hetaing plan named {} in line:{}",
              plan, (*iter).dump());
      throw invalid_argument("Undefined plan.");
    }
    
    shared_ptr<Schedule> schedule = (*heatingPlans)[plan];
    (*roomHeatingPlan)[room] = schedule;
  }
}

void MasterBuilder::buildLogicRules() {
  //NOTE: Order DOES matter, change with caution!
  
  //this should be first rule, probably always
  shared_ptr<SetupSharedStateRule> setupRule = make_shared<SetupSharedStateRule>(logic->getSharedState());
  setupRule->setValue(STATE_WANT_HEATING, 0);
  logic->getRules()->push_back( setupRule );
  
  //this need to be before StoveControlRule
  logic->getRules()->push_back( make_shared<RoomTemperatureRule>(logic) );
  
  //this is probably one of last rules, must be after all rules which control sove behavior
  logic->getRules()->push_back( make_shared<StoveControlRule>(logic->getSharedState()) );
}

void MasterBuilder::buildLogic() {
  spdlog::get(MISC_LOGGER_NAME)->info("Building logic...");
  buildStorage();
  buildSensors();
  buildRelays();
  
  logic = make_shared<Logic>(storage, sensorNetManager, relaysStatesMachine);
  
  buildHeatingPlan();
  spdlog::get(MISC_LOGGER_NAME)->info("Building logic DONE.");
  
  buildLogicRules();
}

void MasterBuilder::buildHttpServer() {
  spdlog::get(MISC_LOGGER_NAME)->info("Building http server...");
  int port = masterConfig[KEY_WEB_SERVER_CONFIG][KEY_WEB_PORT];
  spdlog::get(MISC_LOGGER_NAME)->info("  Http listen port:{}", port);
  httpServer = make_shared<HttpServer>(storage, port);
  httpServer->registerHandler(std::make_shared<HomePlanRestApiHandler>());
  httpServer->registerHandler(std::make_shared<PhysicalSensorRestApiHandler>(logic));
  httpServer->registerHandler(std::make_shared<MeasurementsRestApiHandler>(logic));
  spdlog::get(MISC_LOGGER_NAME)->info("Building http server DONE.");
}


