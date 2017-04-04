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
#include "entities/SensorValue.hpp"
#include "storage/sqlite/SQLiteSensorValueSerializer.hpp"
#include "misc/JSONHelper.hpp"
#include "misc/StringHelper.hpp"

static const time_t LOGIC_THREAD_SLEEP_TIME = 100; //in ms

Logic::Logic(shared_ptr<Storage> store, shared_ptr<SensorNetManager> sensors)
: storage(store),
  sensorNetManager(sensors),
  terminated(false),
  logicThread(nullptr),
  roomHeatingPlan(make_shared<ScheduleMap>()),
  rules(make_shared<LogicRulesVector>()),
  rooms(make_shared<RoomsVector>()),
  logger( spdlog::get(LOGIC_LOGGER_NAME) ),
  sharedState(make_shared<unordered_map<int, int>>()) {

}

Logic::~Logic() {
  terminate();
}

LogicRulesList Logic::getRules() {
  return rules;
}
shared_ptr<ScheduleMap> Logic::getRoomHeatingPlan() {
    return roomHeatingPlan;
}

shared_ptr<SensorNetManager> Logic::getSensorsNetManager() {
  return sensorNetManager;
}

shared_ptr<Storage> Logic::getStorage() {
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
  logger->info("*** Entering main logic loop ***");
  rebuildListOfMeasurementTasks();
  while(true) {
    //Check if thread should finish
    {//critical section
      unique_lock<mutex> lock(logicLock);
      
      if (terminated == true) {
        logger->info("*** Break logic loop ***");
        break;
      }
    }
    
    //Sleep if there is nothing to do
    time_t sleepTime = LOGIC_THREAD_SLEEP_TIME;
    
    shared_ptr<MeasurementTask> task;
    {//critical section
      unique_lock<mutex> lock(logicLock);
      
      task = measurementTasks.top();
    }
    time_t timeToMeasure = task->getTimeToMeasure() * 1000;
    sleepTime = sleepTime > timeToMeasure ? timeToMeasure : sleepTime;
    
    this_thread::sleep_for(chrono::milliseconds(sleepTime));
    
    //fetch measurement
    while(task->getTimeToMeasure() == 0) {
      shared_ptr<PhysicalSensor> sensor = task->getSensor();
      {//critical section
        unique_lock<mutex> lock(logicLock);
        
        measurementTasks.pop();
      }
      
      MeasurementMap data = sensorNetManager->fetchMeasurements(sensor);
      sensor->setLastFetchTime(time(nullptr));
      sensor->setLastMeasurements(data);
      storeMeasurements(sensor->getId(), data);
            
      task = make_shared<MeasurementTask>(sensor);
      {//critical section
        unique_lock<mutex> lock(logicLock);
        
        measurementTasks.push(task);
        
        task = measurementTasks.top();
      }
    }
    
    //execute rules
    for(auto iter = rules->begin(); iter != rules->end(); iter ++) {
      if ((*iter)->isEnabled()) {
        (*iter)->execute();
      }
    }
  }
  
  logger->info("*** Leaving main logic loop ***");
}

SharedState Logic::getSharedState() {
  return sharedState;
}

RoomsList Logic::getRooms() {
  return rooms;
}

void Logic::storeMeasurements(long sensorId, MeasurementMap data) {
  return; //TODO: this is for debug, remove this return
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

void Logic::rebuildListOfMeasurementTasks() {
  logger->info("Rebuilding list of measurement tasks.");
  PhysicalSensorVector list = sensorNetManager->getSensors();
  
  {//critical section
    unique_lock<mutex> lock(logicLock);
    
    //no clear method, so just recreate collection
    measurementTasks = priority_queue<shared_ptr<MeasurementTask>>();
    
    for(auto iter = list.begin(); iter != list.end(); iter++) {
      shared_ptr<PhysicalSensor> sensor = *iter;
      measurementTasks.push( make_shared<MeasurementTask>(sensor));
    }
  }
}

void Logic::terminate() {
  logger->info("Requested termination of logic.");
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
