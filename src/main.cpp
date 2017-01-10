//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <iostream>
#include "mongoose.h"
#include "HttpServer.hpp"
#include "HomePlanRestApiHandler.hpp"
#include "SQLiteStorage.hpp"
#include "Entities.hpp"
#include "SQLiteSerializers.hpp"
#include "SensorNetManager.hpp"
#include "Logic.hpp"
#include "CommunicationLink.hpp"
#include "PhysicalSensorRestApiHandler.hpp"

#include "MeasurementTask.hpp"
#include <queue>

HttpServer* httpServer;
Storage* storage;
shared_ptr<SensorNetManager> sensorNetManager;
shared_ptr<Logic> logic;

void prepareStorage() {
  storage = new SQLiteStorage();
  storage->registerSerializer(SensorValue(), new SQLiteSensorValueSerializer());
  storage->open();
}

void prepareHttpServer() {
  httpServer = new HttpServer(storage);
  httpServer->registerHandler(std::make_shared<HomePlanRestApiHandler>());
  httpServer->registerHandler(std::make_shared<PhysicalSensorRestApiHandler>(logic));
}

void prepareSensors() {
  sensorNetManager = make_shared<SensorNetManager>();
}

void prepareLogic() {
  logic = make_shared<Logic>(storage, sensorNetManager);
}

int main(int argc, const char * argv[]) {
  /*
  priority_queue<shared_ptr<MeasurementTask>> q;
  q.push( make_shared<MeasurementTask>(300));
  q.push( make_shared<MeasurementTask>(350));
  q.push( make_shared<MeasurementTask>(100));
  
  printf("%ld\n", q.top()->getNextMeasurementTime()); q.pop();
  printf("%ld\n", q.top()->getNextMeasurementTime()); q.pop();
  printf("%ld\n", q.top()->getNextMeasurementTime()); q.pop();
  */
  prepareStorage();
  prepareSensors();
  prepareLogic();
  prepareHttpServer();
  
  logic->run();
  httpServer->start();
  return 0;
}
