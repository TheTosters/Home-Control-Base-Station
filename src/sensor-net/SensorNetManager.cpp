//
//  SensorNetManager.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <fstream>
#include <sstream>
#include "sensor-net/SensorNetManager.hpp"
#include "misc/JSONHelper.hpp"
#include "sensor-net/CommunicationLink.hpp"

const string FILE_NAME = "sensors-default.json";

struct PhysicalSensorByIdComparator : public std::unary_function<std::string, bool>
{
  long id;
  
  explicit PhysicalSensorByIdComparator(const long& _id) : id(_id) {}
  
  bool operator() (const shared_ptr<PhysicalSensor> &arg) {
    return arg->getId() == id;
  }
  
};

SensorNetManager::SensorNetManager()
: logger(spdlog::get(COMMUNICATION_LOGGER_NAME)) {
  
}

void SensorNetManager::setSensorsList(PhysicalSensorList sensorsList) {
  sensors = sensorsList;
}

MeasurementMap SensorNetManager::fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count) {
  CommunicationLink link(cltBluetooth, sensor);
  SensorNetProtocolParser parser(&link);
  MeasurementMap result = make_shared<unordered_map<string, MeasurementList>>();
  parser.requestMeasurement(result, count);
  
  return result;
}

void SensorNetManager::saveConfiguration() {
  logger->info("Saving sensors net configuration");
  {//critical section
    unique_lock<mutex> lock(managerMutex);
    json data = toJSON(sensors);
    std::ofstream outputStream(FILE_NAME);
    outputStream << std::setw(4) << data << std::endl;
  }
}

bool SensorNetManager::deleteSensor(long sensorId) {
  //whole metod is critical section
  unique_lock<mutex> lock(managerMutex);
  
  auto posIter = find_if(sensors->begin(), sensors->end(), PhysicalSensorByIdComparator(sensorId));
  if (posIter == sensors->end()) {
    logger->error("Failed to delete sensor with id {}", sensorId);
    return false;
  }
  sensors->erase(posIter);
  logger->info("Removed sensor with id {}", sensorId);
  return true;
}

bool SensorNetManager::addSensor(shared_ptr<PhysicalSensor> sensor) {
  //whole method is critical section
  unique_lock<mutex> lock(managerMutex);
  
  //fail if sensor with used id is registered
  auto posIter = find_if(sensors->begin(), sensors->end(), PhysicalSensorByIdComparator(sensor->getId()));
  if (posIter != sensors->end()) {
    logger->error("Failed to add new sensor - id:{}, name:{}, address:{}",
                 sensor->getId(), sensor->getName(), sensor->getAddress());
    return false;
  }
  
  sensors->push_back(sensor);
  logger->info("Added new sensor - id:{}, name:{}, address:{}",
               sensor->getId(), sensor->getName(), sensor->getAddress());
  return true;
}

shared_ptr<PhysicalSensor> SensorNetManager::getSensorById(long id) {
  auto posIter = find_if(sensors->begin(), sensors->end(), PhysicalSensorByIdComparator(id));
  if (posIter != sensors->end()) {
    return nullptr;
  }
  return *posIter;
}

PhysicalSensorVector SensorNetManager::getSensors() {
  //whole method is critical section
  unique_lock<mutex> lock(managerMutex);
  
  return *sensors;
}
