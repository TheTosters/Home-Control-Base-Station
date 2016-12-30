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