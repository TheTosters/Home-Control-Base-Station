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
#include "SensorValue.hpp"
#include "SQLiteSensorValueSerializer.hpp"

static const time_t LOGIC_THREAD_SLEEP_TIME = 100; //in ms

Logic::Logic(Storage* store, SensorNetManager* sensors)
: storage(store), sensorNetManager(sensors), terminated(false), logicThread(nullptr) {
  
}

Logic::~Logic() {
  terminate();
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