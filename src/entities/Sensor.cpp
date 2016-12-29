//
//  Sensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Sensor.hpp"

#define SERIALIZER_SENSOR_ID  2

Sensor::Sensor()
: Entity(SERIALIZER_SENSOR_ID), posX(0), posY(0), name(""), type(""), lastValue(SensorValue(-1, 0, 0)) {
}

Sensor::Sensor(long _id)
: Entity(_id, SERIALIZER_SENSOR_ID), posX(0), posY(0), name(""), type(""), lastValue(SensorValue(-1, 0, 0)) {
}

string Sensor::getName() {
  return name;
}

void Sensor::setName(string const& name) {
  this->name = name;
}

void Sensor::setAddress(string const& address) {
  this->address = address;
}

string Sensor::getAddress() {
  return address;
}

void Sensor::setType(string const& type) {
  this->type = type;
}

string Sensor::getType() {
  return type;
}

SensorValue Sensor::getLastValue(){
  return lastValue;
}

void Sensor::addValue(SensorValue val){
  //todo:
}

long Sensor::getRoomId() {
  return roomId;
}

void Sensor::setRoomId(long _roomId) {
  roomId = _roomId;
}

void Sensor::setX(int x) {
  posX = x;
}

int Sensor::getX() {
  return posX;
}

void Sensor::setY(int y) {
  posY = y;
}

int Sensor::getY() {
  return posY;
}
