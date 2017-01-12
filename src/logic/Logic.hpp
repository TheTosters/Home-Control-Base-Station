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
#include <unordered_map>
#include "Storage.hpp"
#include "SensorNetManager.hpp"
#include "MeasurementTask.hpp"
#include "Schedule.hpp"
#include "TemperatureIdentifier.hpp"
#include "LogicRule.hpp"
#include "Entities.hpp"

using namespace std;
using namespace nlohmann;

typedef unordered_map<string, shared_ptr<Schedule>> ScheduleMap;

class Logic {
  public:
    Logic(shared_ptr<Storage> storage, shared_ptr<SensorNetManager> sensorNetManager);
    ~Logic();
  
    shared_ptr<Storage> getStorage();
    void run();
    void terminate();
  
    shared_ptr<SensorNetManager> getSensorsNetManager();
    void rebuildListOfMeasurementTasks();
    LogicRulesList getRules();
    shared_ptr<ScheduleMap> getRoomHeatingPlan();
  private:
    shared_ptr<Storage> storage;
    shared_ptr<SensorNetManager> sensorNetManager;
    bool              terminated;
    mutex             logicLock;
    thread*           logicThread;
    priority_queue<shared_ptr<MeasurementTask>> measurementTasks;
    shared_ptr<ScheduleMap> roomHeatingPlan;
    LogicRulesList    rules;
    RoomsList         rooms;
  
    void execute();
    void storeMeasurements(long sensorId, MeasurementMap data);
};

#endif /* Logic_hpp */
