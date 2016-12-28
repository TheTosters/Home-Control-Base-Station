//
//  JSONHelper.cpp
//  HomeControl
//
//  Created by Bartłomiej on 28/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "JSONHelper.hpp"

json toJSON(Point* point) {
  json jsonPoint = {
    {"id", point->getId()},
    {"x", point->getX()},
    {"y", point->getY()}
  };
  return jsonPoint;
}

json toJSON(shared_ptr<Point> point) {
  return toJSON( point.get() );
}

json toJSON(Sensor* sensor) {
  Point* posPtr = sensor->getPosition();
  json pos = toJSON(posPtr);
  
  json jsonSensor = {
    {"id", sensor->getId()},
    {"name", sensor->getName()},
    {"position", pos},
    {"address", sensor->getAddress()},
    //{"type", sensor->getType()}     //TODO: add type to sensor class
  };
  
  return jsonSensor;
}

json toJSON(shared_ptr<Sensor> sensor) {
  return toJSON( sensor.get() );
}

json toJSON(shared_ptr<Room> room) {
  
  json shape = json::array();
  vector<Point>* tmp = room->getShape();
  for(vector<Point>::iterator pointIter = tmp->begin(); pointIter != tmp->end(); pointIter++) {
    json point = toJSON( &(*pointIter) );
    shape += point;
  }
  
  json sensors = json::array();
  vector<Sensor>* tmp2 = room->getSensors();
  for(vector<Sensor>::iterator sensorIter = tmp2->begin(); sensorIter != tmp2->end(); sensorIter++) {
    json sensor = toJSON( &(*sensorIter) );
    sensors += sensor;
  }
  
  json jsonRoom = {
    {"id", room->getId()},
    {"name", room->getName()},
    {"floor", room->getFloor()},
    {"shape", shape},
    {"sensors", sensors}
  };

  return jsonRoom;
}

json toJSON(shared_ptr<SensorValue> sensor) {
  return nullptr;
}
