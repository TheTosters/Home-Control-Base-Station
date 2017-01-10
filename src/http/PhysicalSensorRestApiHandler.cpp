//
//  PhysicalSensorRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "PhysicalSensorRestApiHandler.hpp"
#include "json.hpp"
#include "SensorNetManager.hpp"
#include "JSONHelper.hpp"

using namespace std;
using namespace nlohmann;

PhysicalSensorRestApiHandler::PhysicalSensorRestApiHandler(shared_ptr<Logic> _logic)
: RestApiHandler("/physicalSensors"),
  logic(_logic) {
  
}

void PhysicalSensorRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  PhysicalSensorList list = mgr->getSensors();
  
  json result = toJSON(list);
  string response = result.dump().c_str();
  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            (int) response.length(), response.c_str());
}

void PhysicalSensorRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  //TODO: implement adding
}

void PhysicalSensorRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  long id;
  if (getOrDieQueryVariable(c, data, "id", &id) == false) {
    return;
  }
  
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  if (mgr->deleteSensor(id)) {
    mgr->saveConfiguration();
    logic->rebuildListOfMeasurementTasks();
    noContent(c);
    
  } else {
    notFound(c);
  }
}

