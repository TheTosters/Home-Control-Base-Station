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

using namespace std;
using namespace nlohmann;

typedef unordered_map<string, shared_ptr<Schedule>> ScheduleMap;
typedef shared_ptr<unordered_map<int, int>> SharedState;

class Logic : public SensorDataListener {
  public:
    Logic(shared_ptr<Storage> storage, shared_ptr<SensorNetManager> sensorNetManager);
    virtual ~Logic();
  
    shared_ptr<Storage> getStorage();
    void run();
    void terminate();
  
    shared_ptr<SensorNetManager> getSensorsNetManager();
    void rebuildListOfMeasurementTasks();
    LogicRulesList getRules();
    shared_ptr<ScheduleMap> getRoomHeatingPlan();
    SharedState getSharedState();
    RoomsList getRooms();

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
    SharedState sharedState;   //this is state vector for logic rules
  
    void execute();
    void storeMeasurements(long sensorId, MeasurementMap data);
    void executeMeasurements();
};

#endif /* Logic_hpp */
