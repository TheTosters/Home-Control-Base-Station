//
//  SensorNetManager.hpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SensorNetManager_hpp
#define SensorNetManager_hpp

#include <stdio.h>
#include <vector>
#include <mutex>
#include "entities/PhysicalSensor.hpp"
#include "json/json.hpp"
#include "sensor-net/SensorNetProtocolParser.hpp"
#include "misc/LogHelper.hpp"

using namespace std;
using namespace nlohmann;

class PhysicalSensor;

class SensorNetManager {
  public:
    SensorNetManager();
    PhysicalSensorVector getSensors();
    void setSensorsList(PhysicalSensorList sensorsList);
    MeasurementMap fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count = 1);
    void saveConfiguration();
    bool deleteSensor(long sensorId);
    bool addSensor(shared_ptr<PhysicalSensor> sensor);
    shared_ptr<PhysicalSensor> getSensorById(long id);
  private:
    PhysicalSensorList    sensors;
    mutex                 managerMutex;
    shared_ptr<spdlog::logger> logger;
};
#endif /* SensorNetManager_hpp */
