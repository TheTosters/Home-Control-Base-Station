//
//  PhysicalSensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "PhysicalSensor.hpp"
PhysicalSensor::PhysicalSensor()
: Entity(-1), lastMeasurements(make_shared<vector<shared_ptr<Measurement>>>()) {
  
}

void PhysicalSensor::setName(string const& name) {
  this->name = name;
}

string& PhysicalSensor::getName() {
  return name;
}

void PhysicalSensor::setAddress(string const& address) {
  this->address = address;
}

string& PhysicalSensor::getAddress() {
  return address;
}

void PhysicalSensor::addType(PhysicalSensorType type) {
  this->types.push_back(type);
}

vector<PhysicalSensorType>& PhysicalSensor::getType() {
  return types;
}

void PhysicalSensor::setLastFetchTime(time_t value) {
  lastFetchTime = value;
}

time_t PhysicalSensor::getLastFetchTime() {
  return lastFetchTime;
}

void PhysicalSensor::setDesiredFetchDelay(time_t value) {
  desiredFetchDelay = value;
}

time_t PhysicalSensor::getDesiredFetchDelay() {
  return desiredFetchDelay;
}

void PhysicalSensor::setLastMeasurements(MeasurementMap data) {
  //store values only if timestamp is newer or equal then currently stored
  for(auto iter = data->begin(); iter != data->end(); iter++) {
    MeasurementList valuesList = iter->second;
    
    for(auto iter2 = valuesList->begin(); iter2 != valuesList->end(); iter2++) {
      shared_ptr<Measurement> item = *iter2;
      
      double value;
      SensorValueType valType;
      time_t timeOffset;
      
      tie(valType, value, timeOffset) = *item;
      
      updateLastMeasurement(valType, value, timeOffset);
    }
  }
}

void PhysicalSensor::updateLastMeasurement(SensorValueType valType, double value, time_t time) {
  bool found = false;
  for(auto iter = lastMeasurements->begin(); iter != lastMeasurements->end(); iter++) {

    double tmpValue;
    SensorValueType tmpValType;
    time_t tmpTime;
    
    tie(tmpValType, tmpValue, tmpTime) = *(*iter);
    
    if (tmpValType == valType && tmpTime < time) {
      get<1>( *(*iter) ) = value;
      get<2>( *(*iter) ) = time;
      found = true;
      break;
    }
  }
  
  if (found == false) {
    lastMeasurements->push_back(make_shared<Measurement>(valType, value, time));
  }
}

MeasurementList& PhysicalSensor::getLastMeasurements() {
  return lastMeasurements;
}
