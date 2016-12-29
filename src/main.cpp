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
#include "RoomsRestApiHandler.hpp"

HttpServer* httpServer;
Storage* storage;

void prepareStorage() {
  storage = new SQLiteStorage();
  storage->registerSerializer(Point(), new SQLitePointSerializer());
  storage->registerSerializer(Room(), new SQLiteRoomSerializer());
  storage->registerSerializer(Sensor(), new SQLiteSensorSerializer());
  storage->registerSerializer(SensorValue(), new SQLiteSensorValueSerializer());
  storage->open();
}

void prepareHttpServer() {
  httpServer = new HttpServer(storage);
  httpServer->registerHandler(std::make_shared<HomePlanRestApiHandler>());
  httpServer->registerHandler(std::make_shared<RoomsRestApiHandler>());
}

int main(int argc, const char * argv[]) {
  prepareStorage();
  prepareHttpServer();
  
  
  httpServer->start();
  return 0;
}
