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

static const time_t LOGIC_THREAD_SLEEP_TIME = 100; //in ms

Logic::Logic(Storage* store, SensorNetManager* sensors)
: storage(store), sensorNetManager(sensors), terminated(false) {
  
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
    time_t timeToMeasure = task->getTimeToMeasure();
    sleepTime = sleepTime > timeToMeasure ? timeToMeasure : sleepTime;
    
    this_thread::sleep_for(chrono::milliseconds(sleepTime));
    
    //fetch measurement
    while(task->getTimeToMeasure() == 0) {
      shared_ptr<PhysicalSensor> sensor = task->getSensor();
      measurementTasks.pop();
      
      MeasurementMap data = sensorNetManager->fetchMeasurements(sensor);
      storeMeasurements(sensor, data);
      
      measurementTasks.push(make_shared<MeasurementTask>(sensor));
      
      task = measurementTasks.top();
    }
  }
}

void Logic::storeMeasurements(shared_ptr<PhysicalSensor> sensor, MeasurementMap data) {
  
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