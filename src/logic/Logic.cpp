//
//  Logic.cpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "Logic.hpp"
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include "SensorValue.hpp"
#include "SQLiteSensorValueSerializer.hpp"
#include "JSONHelper.hpp"
#include "StringHelper.hpp"

static const time_t LOGIC_THREAD_SLEEP_TIME = 100; //in ms
const string CONFIG_FILE_NAME = "timeSchedule.json";

Logic::Logic(Storage* store, shared_ptr<SensorNetManager> sensors)
: storage(store),
  sensorNetManager(sensors),
  terminated(false),
  logicThread(nullptr),
  heatingPlans(make_shared<ScheduleMap>()),
  roomHeatingPlan(make_shared<ScheduleMap>()),
  temperatures(make_shared<vector<shared_ptr<TemperatureIdentifier>>>()) {
  
  loadConfig(CONFIG_FILE_NAME);
  buildRules();
}

Logic::~Logic() {
  terminate();
}

void Logic::buildRules() {
  
}

shared_ptr<SensorNetManager> Logic::getSensorsNetManager() {
  return sensorNetManager;
}

void Logic::loadConfig(string const& path) {
  std::ifstream inputFileStream(path);
  if (inputFileStream.good() == false) {
    return;
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  
  json inJson = json::parse(buffer.str());
  
  if (checkIfKeysExists(inJson, {"temperatures", "heatingPlans", "roomsHeating"}) == false){
    fprintf(stderr, "Config, is wrong, expected nodes 'temperatures', 'heatingPlans', 'roomsHeating' in file: %s \n",
            path.c_str());
    throw invalid_argument("Wrong format of config file.");
  }
  
  parseConfigTemperatures(inJson["temperatures"]);
  parseHetingPlans(inJson["heatingPlans"]);
  parseRoomHeating(inJson["roomsHeating"]);
}

void Logic::parseHetingPlans(json const& definition) {
  for(auto iter = definition.begin(); iter != definition.end(); iter++) {
    string planName = iter.key();
    json const& data = iter.value();
    shared_ptr<Schedule> schedule = make_shared<Schedule>(data, temperatures);
    (*heatingPlans)[planName] = schedule;
  }
}

void Logic::parseConfigTemperatures(json const& definition) {
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

void Logic::parseRoomHeating(json const& definition) {
  if (definition.is_array() == false) {
    fprintf(stderr, "Expected array here! \n%s\n", definition.dump().c_str());
    throw invalid_argument("Wrong format of 'roomsHeating' node.");
  }
  
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

Storage* Logic::getStorage() {
  return storage;
}

void Logic::run() {
  {//critical section
    unique_lock<mutex> lock(logicLock);
    if (logicThread != nullptr) {
      return;
    }
    
    terminated = false;
    logicThread = new thread(&Logic::execute, this);
  }
}

//this is main loop for thread
void Logic::execute() {
  buildListOfMeasurementTasks();
  while(true) {
    //Check if thread should finish
    {//critical section
      unique_lock<mutex> lock(logicLock);
      if (terminated == true) {
        break;
      }
    }
    
    //Sleep if there is nothing to do
    time_t sleepTime = LOGIC_THREAD_SLEEP_TIME;
    
    shared_ptr<MeasurementTask> task = measurementTasks.top();
    time_t timeToMeasure = task->getTimeToMeasure() * 1000;
    sleepTime = sleepTime > timeToMeasure ? timeToMeasure : sleepTime;
    
    this_thread::sleep_for(chrono::milliseconds(sleepTime));
    
    //fetch measurement
    while(task->getTimeToMeasure() == 0) {
      shared_ptr<PhysicalSensor> sensor = task->getSensor();
      measurementTasks.pop();
      
      MeasurementMap data = sensorNetManager->fetchMeasurements(sensor);
      sensor->setLastFetchTime(time(nullptr));
      sensor->setLastMeasurements(data);
      storeMeasurements(sensor->getId(), data);
            
      task = make_shared<MeasurementTask>(sensor);
      measurementTasks.push(task);
      
      task = measurementTasks.top();
    }
    
    //execute rules
    for(auto iter = rules.begin(); iter != rules.end(); iter ++) {
      (*iter)->execute();
    }
  }
}

void Logic::storeMeasurements(long sensorId, MeasurementMap data) {
  SQLiteSensorValueSerializer* serializer = storage->requestSerializer<SQLiteSensorValueSerializer>(SensorValue());
  
  for(auto iter = data->begin(); iter != data->end(); iter++) {
    MeasurementList valuesList = iter->second;
    
    for(auto iter2 = valuesList->begin(); iter2 != valuesList->end(); iter2++) {
      shared_ptr<Measurement> item = *iter2;
      
      double value;
      SensorValueType valType;
      time_t time;
      
      tie(valType, value, time) = *item;
      
      SensorValue tmp(-1, sensorId, value, valType, time);
      serializer->store(&tmp);
    }
  }
}

void Logic::buildListOfMeasurementTasks() {
  PhysicalSensorList list = sensorNetManager->getSensors();
  for(auto iter = list.begin(); iter != list.end(); iter++) {
    shared_ptr<PhysicalSensor> sensor = *iter;
    measurementTasks.push( make_shared<MeasurementTask>(sensor));
  }
}

void Logic::terminate() {
  thread* tmp = nullptr;
  
  {//critical section
    unique_lock<mutex> lock(logicLock);
    terminated = true;
    tmp = logicThread;
    logicThread = nullptr;
  }
  
  if (tmp != nullptr) {
    tmp->join();
    delete tmp;
  }
}