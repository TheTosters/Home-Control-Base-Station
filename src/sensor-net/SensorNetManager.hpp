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
#include "PhysicalSensor.hpp"
#include "json.hpp"
#include "SensorNetProtocolParser.hpp"

using namespace std;
using namespace nlohmann;

class PhysicalSensor;

class SensorNetManager {
  public:
    SensorNetManager();
    PhysicalSensorList getSensors();
  
    MeasurementMap fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count = 1);
    void saveConfiguration();
    bool deleteSensor(long sensorId);
    bool addSensor(shared_ptr<PhysicalSensor> sensor);
  private:
    PhysicalSensorList  sensors;
    mutex               managerMutex;
  
    void loadConfiguration();
};
#endif /* SensorNetManager_hpp */
