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

void SensorNetManager::loadConfiguration() {
  std::ifstream inputFileStream(FILE_NAME);
  if (inputFileStream.good() == false) {
    return;
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  sensors = physicalSensorsFromJSON(buffer.str());
}

PhysicalSensorList SensorNetManager::getSensors() {
  return sensors;
}