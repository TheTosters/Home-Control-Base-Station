//
//  SensorNetManager.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <fstream>
#include <sstream>
#include "SensorNetManager.hpp"
#include "JSONHelper.hpp"

const string FILE_NAME = "sensors-default.json";
const long DEFAULT_FETCH_DELAY = 600;

SensorNetManager::SensorNetManager(Storage* stor)
: storage(stor) {
  loadConfiguration();
}

void SensorNetManager::fetchMeasurements() {
  //todo: implement
}

shared_ptr<PhysicalSensor> SensorNetManager::loadSensorConfig(json data) {
  shared_ptr<PhysicalSensor> result = make_shared<PhysicalSensor>();
  
  long tmpLong = getOptionalJSONLong(data, "id");
  if (tmpLong < 0) {
    fprintf(stderr, "Missing mandatory field 'id' in %s", data.dump().c_str());
    return result;
  }
  result->setId(tmpLong);
  
  tmpLong = getOptionalJSONLong(data, "type");
  if (tmpLong < 0) {
    fprintf(stderr, "Missing mandatory field 'type' in %s", data.dump().c_str());
    return result;
  }
  result->setType( static_cast<PhysicalSensorType>(tmpLong));
  
  shared_ptr<string> tmp = getOptionalJSONString(data, "address");
  if (tmp == nullptr) {
    fprintf(stderr, "Missing mandatory field 'address' in %s", data.dump().c_str());
    return result;
  }
  result->setAddress(*tmp);
  
  tmp = getOptionalJSONString(data, "name");
  if (tmp == nullptr) {
    fprintf(stderr, "Missing mandatory field 'name' in %s", data.dump().c_str());
    return result;
  }
  result->setName(*tmp);

  tmpLong = getOptionalJSONLong(data, "fetchDelay");
  tmpLong = tmpLong < 0 ? DEFAULT_FETCH_DELAY : tmpLong;
  result->setDesiredFetchDelay(static_cast<time_t>(tmpLong));
  
  return result;
}

void SensorNetManager::loadConfiguration() {
  std::ifstream inputFileStream(FILE_NAME);
  if (inputFileStream.good() == false) {
    return;
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  
  json inJson = json::parse(buffer.str());
  
  if (inJson.is_array()) {
    for(auto iter = inJson.begin(); iter != inJson.end(); iter++) {
      shared_ptr<PhysicalSensor> sensor = loadSensorConfig(*iter);
      sensors.push_back(sensor);
    }
  }
}

vector<shared_ptr<PhysicalSensor>>& SensorNetManager::getSensors() {
  return sensors;
}