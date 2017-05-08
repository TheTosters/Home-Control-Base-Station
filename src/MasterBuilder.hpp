//
//  MasterBuilder.hpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef MasterBuilder_hpp
#define MasterBuilder_hpp

#include <stdio.h>
#include <string>
#include "logic/Logic.hpp"
#include "http/HttpServer.hpp"
#include "storage/sqlite/SQLiteStorage.hpp"
#include "sensor-net/SensorNetManager.hpp"
#include "json/json.hpp"
#include "logic/TemperatureIdentifier.hpp"
#include <logic/relays/RelaysStatesMachine.hpp>
#include <entities/Entities.hpp>

static const string DEFAULT_CONFIG_FILE = "config-default.json";

class MasterBuilder {
  public:
    MasterBuilder(string const& configFile = DEFAULT_CONFIG_FILE);
  
    void build();
  
    shared_ptr<Logic> getLogic();
    shared_ptr<HttpServer> getHttpServer();
  private:
    bool wasBuilded;
    json masterConfig;
    shared_ptr<SQLiteStorage> storage;
    shared_ptr<Logic> logic;
    shared_ptr<HttpServer> httpServer;
    shared_ptr<SensorNetManager> sensorNetManager;
    shared_ptr<ScheduleMap> heatingPlans;
    TemperatureIdentifierList temperatures;
    shared_ptr<RelaysStatesMachine> relaysStatesMachine;

    void buildLoggers();
    void buildLogic();
    void buildStorage();
    void buildSensors();
    void buildRelays();
    void buildHttpServer();
    void buildHeatingPlan();
    void parseConfigTemperatures(json const& definition);
    void parseHetingPlans(json const& definition);
    void parseRoomHeating(json const& definition);
    void buildLogicRules();
    void buildSingleRule(json const& definition);
    void buildSingleRelay(json const& definition);
    void buildRooms();
//    void buildSingleRoom(json const& definition);
//    shared_ptr<Sensor> buildSingleLogicalSensor(json const& definition);
};

#endif /* MasterBuilder_hpp */
