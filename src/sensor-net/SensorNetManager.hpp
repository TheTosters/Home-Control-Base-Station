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
  private:
    PhysicalSensorList  sensors;
  
    void loadConfiguration();
    shared_ptr<PhysicalSensor> loadSensorConfig(json data);
};
#endif /* SensorNetManager_hpp */
