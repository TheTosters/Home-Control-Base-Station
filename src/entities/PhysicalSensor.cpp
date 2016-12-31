//
//  PhysicalSensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "PhysicalSensor.hpp"
PhysicalSensor::PhysicalSensor()
: Entity(-1) {
  
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

void PhysicalSensor::setType(PhysicalSensorType type) {
  this->type = type;
}

PhysicalSensorType PhysicalSensor::getType() {
  return type;
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

void PhysicalSensor::setLastMeasurementTime(time_t value) {
  lastMeasurementTime = value;
}

time_t PhysicalSensor::getLastMeasurementTime() {
  return lastMeasurementTime;
}

void PhysicalSensor::setLastValue(double value) {
  lastValue = value;
}

double PhysicalSensor::getLastValue() {
  return lastValue;
}

void PhysicalSensor::setPreviousValue(double value) {
  previousValue = value;
}

double PhysicalSensor::getPreviousValue() {
  return previousValue;
}
