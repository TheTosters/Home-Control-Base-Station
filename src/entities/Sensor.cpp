//
//  Sensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Sensor.hpp"

#define SERIALIZER_SENSOR_ID  2

Sensor::Sensor(string _name, Point* pos)
: Entity(SERIALIZER_SENSOR_ID), name(_name), position(pos), lastValue(SensorValue(-1, 0, 0)) {
}

Sensor::Sensor(int _id, string _name, Point* pos)
: Entity(_id, SERIALIZER_SENSOR_ID), name(_name), position(pos), lastValue(SensorValue(-1, 0, 0)) {
}

Point Sensor::getPosition() {
  return position;
}

string Sensor::getName() {
  return name;
}

void Sensor::setPosition(Point p) {
  position = p;
}

void Sensor::setName(string name) {
  this->name = name;
}

SensorValue Sensor::getLastValue(){
  return lastValue;
}

void Sensor::addValue(SensorValue val){
  //todo:
}
