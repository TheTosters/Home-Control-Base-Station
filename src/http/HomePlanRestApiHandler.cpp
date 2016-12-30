//
//  HomePlanRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <vector>

#include "HomePlanRestApiHandler.hpp"
#include "json.hpp"
#include "Storage.hpp"
#include "Entities.hpp"
#include "SQLiteSerializers.hpp"
#include "HttpServer.hpp"
#include "JSONHelper.hpp"
#include "FileRoomsSerializer.hpp"

using namespace std;
using namespace nlohmann;

HomePlanRestApiHandler::HomePlanRestApiHandler() : RestApiHandler("/plans") {
  
}

void HomePlanRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
//  json result = json::array();
//  SQLiteRoomSerializer* serializer = server->getStorage()->requestSerializer<SQLiteRoomSerializer>(Room());
//  auto rooms = serializer->loadAll();
//  
//  for(vector<shared_ptr<Room>>::iterator iter = rooms->begin(); iter != rooms->end(); iter ++) {
//    shared_ptr<Room> roomPtr = *iter;
//    json room = toJSON(roomPtr);
//    result += room;
//  }
  FileRoomsSerializer serializer;
  auto loadedData = serializer.loadAll();
  json result = toJSON(loadedData);
  string response = result.dump().c_str();
  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            (int) response.length(), response.c_str());
}

void HomePlanRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  shared_ptr<string> str = getBodyOrDie(c, data);
  if (str == nullptr) {
    return;
  }
  
  auto tmp = roomListFromJSON(*str);
  FileRoomsSerializer serializer;
  bool succ = serializer.store(tmp);
  if (succ) {
    sendHttpOk(c);
    
  } else {
    internalError(c);
  }
}

void HomePlanRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  methodNotSupported(c, "GET, POST");
}
