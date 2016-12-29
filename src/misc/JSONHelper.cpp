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

  json jsonSensor = {
    {"id", sensor->getId()},
    {"x", sensor->getX()},
    {"y", sensor->getY()},
    {"name", sensor->getName()},
    {"address", sensor->getAddress()},
    {"type", sensor->getType()}
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

bool checkIfKeysExists(json json, vector<string> const& keys) {
  for(auto iter = keys.begin(); iter != keys.end(); iter++) {
    if (json.find(*iter) == json.end()) {
      return false;
    }
  }
  return true;
}

long getOptionalJSONLong(json json, string const& key) {
  if (json.find(key) == json.end()) {
    return -1;
  }
  
  auto value = json[key];
  if (value.is_number() == false) {
    return -1;
  }
  
  return value;
}

shared_ptr<Point> pointFromJSON(json const& json) {
  if (checkIfKeysExists(json, {"x", "y"}) == false) {
    return nullptr;
  }
  long id = getOptionalJSONLong(json, "id");
  
  shared_ptr<Point> result;
  if (id >= 0) {
    result = make_shared<Point>(id, json["x"], json["y"]);
    
  } else {
    result = make_shared<Point>(json["x"], json["y"]);
  }
  return result;
}

shared_ptr<Point> pointFromJSON(string const& data) {
  json json = json::parse(data);
  return pointFromJSON(json);
}

shared_ptr<Sensor> sensorFromJSON(string const& data) {
  json json = json::parse(data);
  if (checkIfKeysExists(json, {"position", "name", "address", "type", "x", "y"}) == false) {
    return nullptr;
  }
  long id = getOptionalJSONLong(json, "id");
  
  shared_ptr<Point> position = pointFromJSON(json["position"]);
  
  shared_ptr<Sensor> result = (id >= 0) ? make_shared<Sensor>(id) : make_shared<Sensor>();
  
  result->setAddress( json["address"] );
  result->setType( json["type"] );
  result->setName( json["name"] );
  result->setX( json["x"] );
  result->setY( json["y"] );
  
  long roomId = getOptionalJSONLong(json, "roomId");
  if (roomId >= 0) {
    result->setRoomId(roomId);
  }
  
  return result;
}

shared_ptr<Room> roomFromJSON(string const& data) {
  json inJson = json::parse(data);
  if (checkIfKeysExists(inJson, {"name", "floor", "shape"}) == false) {
    return nullptr;
  }
  
  json shapeJson = inJson["shape"];
  if (shapeJson.is_array() == false) {
    return nullptr;
  }
  
  long id = getOptionalJSONLong(inJson, "id");
  
  shared_ptr<Room> result;
  if (id >= 0) {
    result = make_shared<Room>(id, inJson["name"] );
    
  } else {
    const string name = inJson["name"];
    result = make_shared<Room>( name );
  }
  result->setFloor(inJson["floor"]);
  
  vector<Point>* shape = result->getShape();
  for(auto iter = shapeJson.begin(); iter != shapeJson.end(); iter++) {
    shared_ptr<Point> point = pointFromJSON(*iter);
    shape->push_back( *point.get() );
  }
  
  auto iter = inJson.find("sensors");
  if (iter != inJson.end()) {
    vector<Sensor>* sensors = result->getSensors();
    json sensorsJson = *iter;
    for(auto iter2 = sensorsJson.begin(); iter2 != sensorsJson.end(); iter2++) {
      shared_ptr<Sensor> sensor = sensorFromJSON(*iter2);
      sensors->push_back( *sensor.get() );
    }
  }
  return result;
}

json toJSON(shared_ptr<SensorValue> sensor) {
  //todo: implement
  return nullptr;
}