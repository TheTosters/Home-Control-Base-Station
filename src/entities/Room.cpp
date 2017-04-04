//
//  Room.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "entities/Room.hpp"
#include "entities/Sensor.hpp"

#define SERIALIZER_ROOM_ID  1

Room::Room()
:Entity(SERIALIZER_ROOM_ID), name(""), floor(0) {
  
}

Room::Room(string _name)
:Entity(SERIALIZER_ROOM_ID), name(_name), floor(0) {
  
}

Room::Room(int id, string _name)
:Entity(id, SERIALIZER_ROOM_ID), name(_name), floor(0) {

}

string Room::getName() {
  return name;
}

void Room::setName(string name) {
  this->name = name;
}

vector<Point>* Room::getShape() {
  return &shape;
}

vector<Sensor>* Room::getSensors() {
  return &sensors;
}

int Room::getFloor() {
  return floor;
}

void Room::setFloor(int _floor) {
  floor = _floor;
}
