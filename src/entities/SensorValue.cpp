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

SensorValue::SensorValue(double _value, time_t _timestamp)
: Entity(SERIALIZER_SENSOR_VALUE_ID), value(_value), timestamp(_timestamp) {
  
}

SensorValue::SensorValue(int id, double _value, time_t _timestamp)
: Entity(id, SERIALIZER_SENSOR_VALUE_ID), value(_value), timestamp(_timestamp) {
  
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