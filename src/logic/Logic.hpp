//
//  Logic.hpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef Logic_hpp
#define Logic_hpp

#include <mutex>
#include <stdio.h>
#include <thread>
#include <queue>
#include "Storage.hpp"
#include "SensorNetManager.hpp"
#include "MeasurementTask.hpp"

using namespace std;

class Logic {
  public:
    Logic(Storage* storage, SensorNetManager* sensorNetManager);
    ~Logic();
  
    Storage*  getStorage();
    void      run();
    void      terminate();
  private:
    Storage*          storage;
    SensorNetManager* sensorNetManager;
    bool              terminated;
    mutex             logicLock;
    thread*           logicThread;
    priority_queue<shared_ptr<MeasurementTask>> measurementTasks;
  
    void execute();
    void buildListOfMeasurementTasks();
    void storeMeasurements(long sensorId, MeasurementMap data);
};

#endif /* Logic_hpp */
