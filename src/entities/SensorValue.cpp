//
//  SensorValue.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SensorValue.hpp"

#define SERIALIZER_SENSOR_VALUE_ID  3

SensorValue::SensorValue()
: Entity(SERIALIZER_SENSOR_VALUE_ID), value(0), timestamp(0) {
  
}

SensorValue::SensorValue(long id, long sensId, double _value, SensorValueType valType, time_t _timestamp)
: Entity(id, SERIALIZER_SENSOR_VALUE_ID), physicalSensorId(sensId), value(_value), timestamp(_timestamp), valueType(valType) {
  
}

double SensorValue::getValue() {
  return value;
}

time_t SensorValue::getTimestamp() {
  return timestamp;
}

void SensorValue::setValue(double val) {
  value = val;
}

void SensorValue::setTimestamp(time_t time) {
  timestamp = time;
}

long SensorValue::getPhysicalSensorId() {
  return physicalSensorId;
}

void SensorValue::setPhysicalSensorId(long sensorId) {
  this->physicalSensorId = sensorId;
}

SensorValueType SensorValue::getValueType() {
  return valueType;
}

void SensorValue::setValueType(SensorValueType value) {
  this->valueType = value;
}
