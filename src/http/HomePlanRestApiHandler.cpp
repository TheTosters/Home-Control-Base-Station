//
//  HomePlanRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "HomePlanRestApiHandler.hpp"
#include "json.hpp"
#include "Storage.hpp"
#include "Entities.hpp"
#include "SQLiteSerializers.hpp"
#include "HttpServer.hpp"
#include <vector>

using namespace std;
using namespace nlohmann;

HomePlanRestApiHandler::HomePlanRestApiHandler() : RestApiHandler("/plans") {
  
}

void HomePlanRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  json result = json::array();
  SQLiteRoomSerializer* serializer = server->getStorage()->requestSerializer<SQLiteRoomSerializer>(Room());
  auto rooms = serializer->loadAll();
  
  for(vector<shared_ptr<Room>>::iterator iter = rooms->begin(); iter != rooms->end(); iter ++) {
    shared_ptr<Room> roomPtr = *iter;
    
    json shape = json::array();
    vector<Point>* tmp = roomPtr->getShape();
    for(vector<Point>::iterator pointIter = tmp->begin(); pointIter != tmp->end(); pointIter++) {
      json point = {
        {"id", pointIter->getId()},
        {"x", pointIter->getX()},
        {"y", pointIter->getY()}
      };
      shape += point;
    }
    
    json sensors = json::array();
    vector<Sensor>* tmp2 = roomPtr->getSensors();
    for(vector<Sensor>::iterator sensorIter = tmp2->begin(); sensorIter != tmp2->end(); sensorIter++) {
      Point* posPtr = sensorIter->getPosition();
      json pos = {
        {"id", posPtr->getId()},
        {"x", posPtr->getX()},
        {"y", posPtr->getY()}
      };
      
      json sensor = {
        {"id", sensorIter->getId()},
        {"name", sensorIter->getName()},
        {"position", pos},
        {"address", sensorIter->getAddress()},
        //{"type", sensorIter->getType()}     //TODO: add type to sensor class
      };
      
      sensors += sensor;
    }
    
    json room = {
      {"name", roomPtr->getName()},
      {"floor", roomPtr->getFloor()},
      {"shape", shape},
      {"sensors", sensors}
    };
    result += room;
  }
  
  string response = result.dump().c_str();
  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            (int) response.length(), response.c_str());
}

void HomePlanRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  notImplemented(c);
}

void HomePlanRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  notImplemented(c);
}
