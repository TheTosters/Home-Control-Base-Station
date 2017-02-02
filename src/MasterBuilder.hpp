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

    void buildLoggers();
    void buildLogic();
    void buildStorage();
    void buildSensors();
    void buildHttpServer();
    void buildHeatingPlan();
    void parseConfigTemperatures(json const& definition);
    void parseHetingPlans(json const& definition);
    void parseRoomHeating(json const& definition);
    void buildLogicRules();
};

#endif /* MasterBuilder_hpp */
