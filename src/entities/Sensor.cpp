//
//  Sensor.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "entities/Sensor.hpp"

#define SERIALIZER_SENSOR_ID  2

Sensor::Sensor(long _id)
: Entity(_id, SERIALIZER_SENSOR_ID), posX(0), posY(0), name(""), physicalSensorId(-1), roomId(-1) {
}

string Sensor::getName() {
  return name;
}

void Sensor::setName(string const& name) {
  this->name = name;
}

void Sensor::setPhysicalSensorId(long id) {
  this->physicalSensorId = id;
}

long Sensor::getPhysicalSensorId() {
  return physicalSensorId;
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
