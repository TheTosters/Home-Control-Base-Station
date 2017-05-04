//
//  JSONHelper.cpp
//  HomeControl
//
//  Created by Bartłomiej on 28/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "misc/JSONHelper.hpp"
#include "misc/LogHelper.hpp"
#include <tuple>
#include "entities/SensorValue.hpp"

const long PHYSICAL_SENSOR_DEFAULT_FETCH_DELAY = 600;

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
    {"physicalSensorId", sensor->getPhysicalSensorId()},
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

json measurementsToJSON(PhysicalSensorList const& list) {
  json result = json::array();
  for (auto iter = list->begin(); iter != list->end(); iter++) {
    MeasurementList mList = (*iter)->getLastMeasurements();

    for (auto mIter : *mList) {
      SensorValueType type = std::get<0>(*(mIter));
      double value = std::get<1>(*(mIter));
      time_t timestamp = std::get<2>(*(mIter));
      json item = {
          { "id", (*iter)->getId() },
          { "type", static_cast<int>(type) },
          { "value", value },
          { "timestamp", timestamp }
      };

      result += item;
    }
  }
  return result;
}

json toJSON(PhysicalSensorList const& list) {
  json result = json::array();
  for(auto iter = list->begin(); iter != list->end(); iter++) {
    
    json types = json::array();
    vector<PhysicalSensorType>& tmp = (*iter)->getType();
    for(auto iter2 = tmp.begin(); iter2 != tmp.end(); iter2++) {
      types += *iter2;
    }
    
    PhysicalSensorMetaData* tmpMeta = (*iter)->getMetadata();

    json meta = {
        {"swVersion", tmpMeta->softwareVersion},
        {"powerMode", static_cast<int>(tmpMeta->powerMode)},
        {"powerActivity", static_cast<int>(tmpMeta->powerActivity)},
        {"powerPeroid", tmpMeta->powerPeroid},
        {"temperatureResolution", tmpMeta->temperatureResolution},
        {"temperaturePeriod", tmpMeta->temperaturePeriod},
        {"nodeSystemTime", tmpMeta->nodeSystemTime}
    };

    json item = {
      {"id", (*iter)->getId()},
      {"name", (*iter)->getName()},
      {"address", (*iter)->getAddress()},
      {"fetchDelay", (*iter)->getDesiredFetchDelay()},
      {"type", types},
      {"meta", meta}
    };
    
    result += item;
  }
  return result;
}

bool checkIfKeysExists(json json, vector<string> const& keys, string* missing) {
  for(auto iter = keys.begin(); iter != keys.end(); iter++) {
    if (json.find(*iter) == json.end()) {
      if (missing != nullptr) {
        *missing = *iter;
      }
      return false;
    }
  }
  return true;
}

void getOptionalJSONBool(json json, string const& key, bool& resultValue) {
  if (json.find(key) == json.end()) {
    return;
  }
  
  auto value = json[key];
  if (value.is_boolean() == false) {
    return;
  }
  resultValue = value;
}

shared_ptr<string> getOptionalJSONString(json json, string const& key) {
  if (json.find(key) == json.end()) {
    return nullptr;
  }
  
  auto value = json[key];
  if (value.is_string() == false) {
    return nullptr;
  }
  string s = value;
  return make_shared<string>(s);
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

shared_ptr<PhysicalSensor> physicalSensorFromJSON(json const& data) {
  shared_ptr<PhysicalSensor> result = make_shared<PhysicalSensor>();
  
  long tmpLong = getOptionalJSONLong(data, "id");
  if (tmpLong < 0) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'id' in {}", data.dump());
    return result;
  }
  result->setId(tmpLong);
  
  if (data.find("type") == data.end()) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'type' in {}", data.dump());
    return result;
  }
  json types = data["type"];
  if (types.is_array() == false) {
    spdlog::get(MISC_LOGGER_NAME)->error("Mandatory field 'type' must be an array in {}", data.dump());
    return result;
  }
  for(auto iter = types.begin(); iter != types.end(); iter++){
    int t = *iter;
    result->addType( static_cast<PhysicalSensorType>(t));
  }
  
  shared_ptr<string> tmp = getOptionalJSONString(data, "address");
  if (tmp == nullptr) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'address' in {}", data.dump());
    return result;
  }
  result->setAddress(*tmp);
  
  tmp = getOptionalJSONString(data, "name");
  if (tmp == nullptr) {
    spdlog::get(MISC_LOGGER_NAME)->error("Missing mandatory field 'name' in {}", data.dump());
    return result;
  }
  result->setName(*tmp);
  
  tmpLong = getOptionalJSONLong(data, "fetchDelay");
  tmpLong = tmpLong <= 0 ? PHYSICAL_SENSOR_DEFAULT_FETCH_DELAY : tmpLong;
  result->setDesiredFetchDelay(static_cast<time_t>(tmpLong));
  
  //Meta data
  if (data.find("meta") != data.end()) {
    PhysicalSensorMetaData* tmpMeta = result->getMetadata();
    json metaJson = data["meta"];

    tmp = getOptionalJSONString(metaJson, "softwareVersion");
    tmpMeta->softwareVersion = tmp ? *tmp : "";
    tmpMeta->powerMode = static_cast<PhysicalSensorPowerSaveMode>(getOptionalJSONLong(metaJson, "powerMode"));
    tmpMeta->powerActivity = static_cast<PhysicalSensorPowerSaveActivity>(getOptionalJSONLong(metaJson, "powerActivity"));
    tmpMeta->powerPeroid = static_cast<int>(getOptionalJSONLong(metaJson, "powerPeroid"));
    tmpMeta->temperatureResolution = static_cast<int>(getOptionalJSONLong(metaJson, "temperatureResolution"));
    tmpMeta->temperaturePeriod = static_cast<int>(getOptionalJSONLong(metaJson, "temperaturePeriod"));
    tmpMeta->nodeSystemTime = getOptionalJSONLong(metaJson, "nodeSystemTime");
  }

  return result;
}

PhysicalSensorList physicalSensorsFromJSON(string const& data) {
  PhysicalSensorList result = make_shared<PhysicalSensorVector>();
  json inJson = json::parse(data);
  
  if (inJson.is_array()) {
    for(auto iter = inJson.begin(); iter != inJson.end(); iter++) {
      shared_ptr<PhysicalSensor> sensor = physicalSensorFromJSON(*iter);
      result->push_back(sensor);
    }
  }
  
  return result;
}

shared_ptr<Point> pointFromJSON(string const& data) {
  json json = json::parse(data);
  return pointFromJSON(json);
}

shared_ptr<Sensor> sensorFromJSON(json const& json) {
  if (checkIfKeysExists(json, {"id", "name", "physicalSensorId", "x", "y"}) == false) {
    return nullptr;
  }
  
  long id = json["id"];
  shared_ptr<Sensor> result = make_shared<Sensor>(id);
  
  result->setName( json["name"] );
  result->setX( json["x"] );
  result->setY( json["y"] );
  
  long sensId = getOptionalJSONLong(json, "physicalSensorId");
  if (sensId >= 0) {
    result->setPhysicalSensorId(sensId);
  }
  
  long roomId = getOptionalJSONLong(json, "roomId");
  if (roomId >= 0) {
    result->setRoomId(roomId);
  }
  
  return result;
}

shared_ptr<Sensor> sensorFromJSON(string const& data) {
  json json = json::parse(data);
  return sensorFromJSON(json);
}

shared_ptr<Room> roomFromJSON(json const& inJson) {
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
      if (sensor) {
        sensor->setRoomId(id);
        sensors->push_back( *sensor.get() );
      }
    }
  }
  return result;
}

shared_ptr<Room> roomFromJSON(string const& data) {
  json json = json::parse(data);
  return roomFromJSON(json);
}

json toJSON(shared_ptr<SensorValue> sensorValue, bool includeSensorId) {
  json result = {
    {"id", sensorValue->getId()},
    {"value", sensorValue->getValue()},
    {"timestamp", static_cast<long>(sensorValue->getTimestamp())},
    {"valueType", static_cast<int>(sensorValue->getValueType())}
  };
  if (includeSensorId) {
    result["sensorId"] = sensorValue->getPhysicalSensorId();
  }
  return result;
}

json toJSON(SensorValueList const& list, bool includeSensorId) {
  json result = json::array();
  for(auto iter = list->begin(); iter != list->end(); iter++) {
    result += toJSON(*iter, includeSensorId);
  }
  return result;
}

RoomsList roomListFromJSON(string const& data) {
  json inJson = json::parse(data);
  auto result = make_shared<vector<shared_ptr<Room>>>();
  
  if (inJson.is_array()) {
    for(auto iter = inJson.begin(); iter != inJson.end(); iter++) {
      shared_ptr<Room> room = roomFromJSON(*iter);
      result->push_back(room);
    }
  }
  
  return result;
}

json toJSON(RoomsList roomsList) {
  json result = json::array();
  
  for(vector<shared_ptr<Room>>::iterator iter = roomsList->begin(); iter != roomsList->end(); iter ++) {
    shared_ptr<Room> roomPtr = *iter;
    json room = toJSON(roomPtr);
    result += room;
  }
  
  return result;
}
