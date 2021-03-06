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
#include "storage/Storage.hpp"
#include "sensor-net/SensorNetManager.hpp"
#include "logic/MeasurementTask.hpp"
#include "logic/Schedule.hpp"
#include "logic/TemperatureIdentifier.hpp"
#include "logic/rules/LogicRule.hpp"
#include "entities/Entities.hpp"
#include "misc/LogHelper.hpp"
#include "sensor-net/SensorDataListener.hpp"
#include <logic/relays/RelaysStatesMachine.hpp>
#include <logic/SharedState.h>

using namespace std;
using namespace nlohmann;

typedef unordered_map<string, shared_ptr<Schedule>> ScheduleMap;

class Logic : public SensorDataListener {
  public:
    Logic(shared_ptr<Storage> storage, shared_ptr<SensorNetManager> sensorNetManager,
        shared_ptr<RelaysStatesMachine> relaysStatesMachine);
    virtual ~Logic();
  
    shared_ptr<Storage> getStorage();
    void run();
    void terminate();
  
    shared_ptr<SensorNetManager> getSensorsNetManager();
    void rebuildListOfMeasurementTasks();
    LogicRulesList getRules();
    shared_ptr<ScheduleMap> getRoomHeatingPlan();
    shared_ptr<SharedState> getSharedState();
    RoomsList getRooms();
    shared_ptr<RelaysStatesMachine> getRelaysStatesMachine();
    void setRulesConfigFile(const string& path);

    virtual void onSensorData(shared_ptr<PhysicalSensor> sensor, MeasurementMap measurements) override;
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
    shared_ptr<spdlog::logger> logger;
    shared_ptr<SharedState> sharedState;   //this is state vector for logic rules
    shared_ptr<RelaysStatesMachine> relaysStatesMachine;
    string rulesConfigPath;

    void execute();
    void storeMeasurements(long sensorId, MeasurementMap data);
    void executeMeasurements();
};

#endif /* Logic_hpp */
