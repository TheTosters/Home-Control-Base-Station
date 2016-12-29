//
//  RoomsRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 28/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "RoomsRestApiHandler.hpp"
#include "Entities.hpp"
#include "SQLiteSerializers.hpp"
#include "HttpServer.hpp"
#include "JSONHelper.hpp"

RoomsRestApiHandler::RoomsRestApiHandler() : RestApiHandler("/rooms") {
  
}

void RoomsRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  long roomId;
  if (getOrDieQueryVariable(c, data, "id", &roomId) == false) {
    return;
  }
  
  SQLiteRoomSerializer* serializer = server->getStorage()->requestSerializer<SQLiteRoomSerializer>(Room());
  auto room = serializer->load(roomId);
  if (room == nullptr) {
    notFound(c);
    return;
  }
  
  json result = toJSON(room);
  
  string response = result.dump().c_str();
  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            (int) response.length(), response.c_str());
}

void RoomsRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  shared_ptr<string> body = getBodyOrDie(c, data);
  if (body == nullptr) {
    return;
  }
  
  shared_ptr<Room> room = roomFromJSON(*body);
  SQLiteRoomSerializer* serializer = server->getStorage()->requestSerializer<SQLiteRoomSerializer>(Room());
  serializer->storeOrUpdate( room.get() );
  sendHttpOk(c);
}

void RoomsRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  long roomId;
  if (getOrDieQueryVariable(c, data, "id", &roomId) == false) {
    return;
  }
  
  long dropSensorsLong = 0;
  bool hasDropSensors = getQueryVariable(data, "dropSensors", &dropSensorsLong);
  bool dropSensors = hasDropSensors && (dropSensorsLong == 1);
  
  SQLiteRoomSerializer* serializer = server->getStorage()->requestSerializer<SQLiteRoomSerializer>(Room());
  bool found = serializer->remove(roomId, dropSensors);
  
  if (found) {
    sendHttpOk(c);
    
  } else {
    notFound(c);
  }
  
}
