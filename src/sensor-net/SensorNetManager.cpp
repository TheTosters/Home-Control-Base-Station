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
#include "CommunicationLink.hpp"

const string FILE_NAME = "sensors-default.json";
const long DEFAULT_FETCH_DELAY = 600;

struct PhysicalSensorByIdComparator : public std::unary_function<std::string, bool>
{
  long id;
  
  explicit PhysicalSensorByIdComparator(const long& _id) : id(_id) {}
  
  bool operator() (const shared_ptr<PhysicalSensor> &arg) {
    return arg->getId() == id;
  }
  
};

SensorNetManager::SensorNetManager() {
  loadConfiguration();
}

MeasurementMap SensorNetManager::fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count) {
  CommunicationLink link(cltBluetooth, sensor);
  SensorNetProtocolParser parser(&link);
  MeasurementMap result = make_shared<unordered_map<string, MeasurementList>>();
  parser.requestMeasurement(result, count);
  
  return result;
}

void SensorNetManager::saveConfiguration() {
  json data = toJSON(sensors);
  std::ofstream outputStream(FILE_NAME);
  outputStream << std::setw(4) << data << std::endl;
}

bool SensorNetManager::deleteSensor(long sensorId) {
  auto posIter = find_if(sensors.begin(), sensors.end(), PhysicalSensorByIdComparator(sensorId));
  if (posIter == sensors.end()) {
    return false;
  }
  sensors.erase(posIter);
  return true;
}

shared_ptr<PhysicalSensor> SensorNetManager::loadSensorConfig(json data) {
  shared_ptr<PhysicalSensor> result = make_shared<PhysicalSensor>();
  
  long tmpLong = getOptionalJSONLong(data, "id");
  if (tmpLong < 0) {
    fprintf(stderr, "Missing mandatory field 'id' in %s\n", data.dump().c_str());
    return result;
  }
  result->setId(tmpLong);
  
  if (data.find("type") == data.end()) {
    fprintf(stderr, "Missing mandatory field 'type' in %s\n", data.dump().c_str());
    return result;
  }
  json types = data["type"];
  if (types.is_array() == false) {
    fprintf(stderr, "Mandatory field 'type' must be an array in %s\n", data.dump().c_str());
    return result;
  }
  for(auto iter = types.begin(); iter != types.end(); iter++){
    int t = *iter;
    result->addType( static_cast<PhysicalSensorType>(t));
  }
  
  shared_ptr<string> tmp = getOptionalJSONString(data, "address");
  if (tmp == nullptr) {
    fprintf(stderr, "Missing mandatory field 'address' in %s\n", data.dump().c_str());
    return result;
  }
  result->setAddress(*tmp);
  
  tmp = getOptionalJSONString(data, "name");
  if (tmp == nullptr) {
    fprintf(stderr, "Missing mandatory field 'name' in %s\n", data.dump().c_str());
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

PhysicalSensorList SensorNetManager::getSensors() {
  return sensors;
}