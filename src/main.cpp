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

#include "CommunicationLink.hpp"

HttpServer* httpServer;
Storage* storage;
SensorNetManager* sensorNetManager;

void prepareStorage() {
  storage = new SQLiteStorage();
  storage->registerSerializer(SensorValue(), new SQLiteSensorValueSerializer());
  storage->open();
}

void prepareHttpServer() {
  httpServer = new HttpServer(storage);
  httpServer->registerHandler(std::make_shared<HomePlanRestApiHandler>());
}

void prepareSensors() {
  sensorNetManager = new SensorNetManager(storage);
}

int main(int argc, const char * argv[]) {
  prepareStorage();
  prepareSensors();
  prepareHttpServer();
  
  
  httpServer->start();
  return 0;
}
