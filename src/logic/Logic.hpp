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

using namespace std;
using namespace nlohmann;

typedef unordered_map<string, shared_ptr<Schedule>> ScheduleMap;

class Logic {
  public:
    Logic(Storage* storage, shared_ptr<SensorNetManager> sensorNetManager);
    ~Logic();
  
    Storage*  getStorage();
    void      run();
    void      terminate();
  
    shared_ptr<SensorNetManager> getSensorsNetManager();
    void rebuildListOfMeasurementTasks();
  private:
    Storage*          storage;
    shared_ptr<SensorNetManager> sensorNetManager;
    bool              terminated;
    mutex             logicLock;
    thread*           logicThread;
    priority_queue<shared_ptr<MeasurementTask>> measurementTasks;
    shared_ptr<ScheduleMap> heatingPlans;
    shared_ptr<ScheduleMap> roomHeatingPlan;
    TemperatureIdentifierList temperatures;
    LogicRulesList    rules;
  
    void execute();
    void storeMeasurements(long sensorId, MeasurementMap data);
  
    void buildRules();
    void loadConfig(string const& path);
    void parseConfigTemperatures(json const& definition);
    void parseHetingPlans(json const& definition);
    void parseRoomHeating(json const& definition);
};

#endif /* Logic_hpp */