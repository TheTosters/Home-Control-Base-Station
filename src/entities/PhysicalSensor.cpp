//
//  PhysicalSensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <algorithm>
#include "entities/PhysicalSensor.hpp"
#include "misc/ConversionHelper.hpp"

const unordered_set<PhysicalSensorType, hash<int>> KNOWN_PHYSICAL_SENSOR_TYPES = {
    PhysicalSensorType_TEMPERATURE, PhysicalSensorType_POWER_SOURCE_LEVEL
};

PhysicalSensorMetaData::PhysicalSensorMetaData()
: softwareVersion(""),
  powerMode(PhysicalSensorPowerSaveMode_1),
  powerActivity(PhysicalSensorPowerSaveActivity_1),
  powerPeroid(0),
  temperatureResolution(12),
  temperaturePeriod(0), nodeSystemTime(0) {

}

PhysicalSensor::PhysicalSensor()
: Entity(-1),
  lastFetchTime(0),
  desiredFetchDelay(0),
  lastMeasurements(make_shared<vector<shared_ptr<Measurement>>>()),
  metaData(new PhysicalSensorMetaData()){
  
}

PhysicalSensor::~PhysicalSensor() {
  delete metaData;
  metaData = nullptr;
}

PhysicalSensorMetaData* PhysicalSensor::getMetadata() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return metaData;
}

void PhysicalSensor::setName(string const& name) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  this->name = name;
}

string PhysicalSensor::getName() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return name;
}

void PhysicalSensor::setAddress(string const& address) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  this->address = address;
}

string PhysicalSensor::getAddress() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return address;
}

void PhysicalSensor::addType(PhysicalSensorType type) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  this->types.push_back(type);
}

vector<PhysicalSensorType> PhysicalSensor::getType() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return types;
}

void PhysicalSensor::setLastFetchTime(time_t value) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  lastFetchTime = value;
}

time_t PhysicalSensor::getLastFetchTime() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return lastFetchTime;
}

void PhysicalSensor::setDesiredFetchDelay(time_t value) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  desiredFetchDelay = value;
}

time_t PhysicalSensor::getDesiredFetchDelay() {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return desiredFetchDelay;
}

void PhysicalSensor::setLastMeasurements(MeasurementMap data) {
  //store values only if timestamp is newer or equal then currently stored
  lock_guard<recursive_mutex> lock(sensorMutex);
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
    
    if (tmpValType == valType) {
      found = true;
      if (tmpTime < time) {
        get<1>( *(*iter) ) = value;
        get<2>( *(*iter) ) = time;
      }
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

bool PhysicalSensor::isType(PhysicalSensorType type) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  return find(types.begin(), types.end(), type) != types.end();
}

shared_ptr<Measurement> PhysicalSensor::getLastMeasurement(PhysicalSensorType type) {
  lock_guard<recursive_mutex> lock(sensorMutex);
  for(auto msrPtr : *lastMeasurements) {
    SensorValueType valType = get<0>( *(msrPtr) );
    if (toPhysicalSensorType(valType) == type) {
      return msrPtr;
    }
  }
  return nullptr;
}

void PhysicalSensor::lock() {
  sensorMutex.lock();
}

void PhysicalSensor::unlock() {
  sensorMutex.unlock();
}
