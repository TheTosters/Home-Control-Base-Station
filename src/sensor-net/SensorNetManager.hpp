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
#include "Storage.hpp"

using namespace std;
using namespace nlohmann;

class PhysicalSensor;

class SensorNetManager {
  public:
    SensorNetManager(Storage* storage);
    vector<shared_ptr<PhysicalSensor>>& getSensors();
  
    void fetchMeasurements();
  private:
    vector<shared_ptr<PhysicalSensor>>  sensors;
    Storage*  storage;
  
    void loadConfiguration();
    shared_ptr<PhysicalSensor> loadSensorConfig(json data);
};
#endif /* SensorNetManager_hpp */