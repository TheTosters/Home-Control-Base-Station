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
#include "SensorNetProtocolParser.hpp"
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

SensorNetManager::SensorNetManager()
: logger(spdlog::get(COMMUNICATION_LOGGER_NAME)), scannedSensors(nullptr), hciWrapper(nullptr), resolverThread(nullptr) {

}

SensorNetManager::~SensorNetManager() {
  if (hciWrapper != nullptr) {
    delete hciWrapper;
    hciWrapper = nullptr;
  }

  if (scanningThread != nullptr) {
    scanningThread->join();
    delete scanningThread;
    scanningThread = nullptr;
  }

  if (resolverThread != nullptr) {
    thread* tmp = resolverThread;

    //if thread is working, then this informs it to stop
    unique_lock<mutex> lock(managerMutex);
    resolverThread = nullptr; //this is check in resolverThreadMain
    lock.unlock();

    tmp->join();
    delete tmp;
  }
}

void SensorNetManager::setSensorsList(PhysicalSensorList sensorsList) {
  sensors = sensorsList;
}

MeasurementMap SensorNetManager::fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count) {
  MeasurementMap result = nullptr;
  try {
    CommunicationLink link(cltBluetooth, sensor, logger);
    if (link.isConnected()) {
      SensorNetProtocolParser parser(&link);
      parser.sendPreamble();
      result = make_shared<unordered_map<string, MeasurementList>>();
      parser.requestMeasurement(result, count);
    }
  } catch(std::exception const& e) {
    logger->error("Exception at fetchMeasurements:");
    logger->error(e.what());

  } catch (...) {
    logger->error("Undefined exception at fetchMeasurements");
  }
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

SensorNetManagerStartScanResult SensorNetManager::scanForSensors() {
  //whole method is critical section
  unique_lock<mutex> lock(managerMutex);
  if (resolverThread != nullptr) {
    return SensorNetManagerStartScanResult_ALREADY_IN_PROGRESS;
  }

  if (hciWrapper != nullptr) {
    return SensorNetManagerStartScanResult_ALREADY_IN_PROGRESS;
  }
  hciWrapper = new HciWrapper(*this);

  if (hciWrapper->startScan() == true) {
    //start thread
    scanningThread = new thread(&SensorNetManager::scanningThreadMain, this);
    return SensorNetManagerStartScanResult_START;

  } else {
    delete hciWrapper;
    hciWrapper = nullptr;
    scannedSensors = nullptr;
    return SensorNetManagerStartScanResult_FAILED;
  }
}

void SensorNetManager::onScanStart() {
  logger->info("Start BTLE scan for physical sensors");
  scannedDevices.clear();
}

void SensorNetManager::onScanStop() {
  //whole method is critical section
  unique_lock<mutex> lock(managerMutex);
  logger->info("BTLE scan for physical sensors finished, now resolving {} devices", scannedDevices.size());
  scannedSensors = make_shared<PhysicalSensorVector>();
  if (scannedDevices.size() > 0) {
    logger->info("Starting thread to resolve nodes informations.");
    resolverThread = new thread(&SensorNetManager::resolverThreadMain, this);
  }
}

void SensorNetManager::onNewDeviceFound(const BTLEDevice& device) {
  scannedDevices.push_back(device);
}

SensorNetManagerScanStatus SensorNetManager::getCurrentScanStatus() {
  unique_lock<mutex> lock(managerMutex);
    if (resolverThread != nullptr) {
      return SensorNetManagerScanStatus_IN_PROGRESS;
    }

    if (hciWrapper != nullptr) {
      return SensorNetManagerScanStatus_IN_PROGRESS;
    }

    return (scannedSensors == nullptr) || (scannedSensors->size() == 0) ? SensorNetManagerScanStatus_NO_RESULT :
        SensorNetManagerScanStatus_FINISHED_WITH_RESULT;
}

PhysicalSensorList SensorNetManager::getScannedPhysicalSensors() {
  PhysicalSensorList result = make_shared<PhysicalSensorVector>(*scannedSensors);
  return result;
}

void SensorNetManager::scanningThreadMain() {
  logger->info("Scanning thread started.");
  if (hciWrapper) {
    const int SCAN_COUNT = 5;
    for(int t = 0; t < SCAN_COUNT; t++) {
      logger->info("Scanning loop {}/{}.",t,SCAN_COUNT);
      hciWrapper->scanLoop();
    }
  }
  hciWrapper->stopScan();

  logger->info("Scanning thread finished.");
}

void SensorNetManager::resolverThreadMain() {
  logger->info("Resolver thread started.");
  if (scanningThread != nullptr) {
    scanningThread->join();
    delete scanningThread;
    scanningThread = nullptr;
  }

  while (true) {
    shared_ptr<PhysicalSensor> sensor = make_shared<PhysicalSensor>();
    {
      //critical section
      unique_lock<mutex> lock(managerMutex);
      if (resolverThread == nullptr || scannedDevices.size() == 0) {
        lock.unlock();
        break;
      }
      BTLEDevice dev = scannedDevices.back();
      scannedDevices.pop_back();
      sensor->setAddress(dev.address);
      sensor->setName(dev.name);
    }

    logger->info("Resolving: {}", sensor->getAddress());
    try {
      CommunicationLink link(cltBluetooth, sensor, logger);
      if (link.isConnected()) {
        SensorNetProtocolParser parser(&link);
        parser.sendPreamble();
        if (parser.requestSensorSpec() == true) {
          unique_lock<mutex> lock(managerMutex);
          scannedSensors->push_back(sensor);
        } else {
          logger->warn("Looks like device {} is not compatible node.", sensor->getAddress());
        }
      } else {
        logger->warn("Connection attempt failed to resolved device: {}", sensor->getAddress());
      }
    } catch (std::exception const& e) {
      //those might occur if trying to connect to some btle devices which are not nodes
      logger->error("Exception at resolverThreadMain:");
      logger->error(e.what());

    } catch (...) {
      logger->error("Undefined exception at resolverThreadMain");
    }
  }

  {
    //critical section
    unique_lock<mutex> lock(managerMutex);
    resolverThread->detach();
    delete resolverThread;
    resolverThread = nullptr;

    delete hciWrapper;
    hciWrapper = nullptr;
  }
  logger->info("Resolver thread done.");
}
