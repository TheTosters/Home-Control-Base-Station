//
//  PhysicalSensorRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "PhysicalSensorRestApiHandler.hpp"
#include "sensor-net/SensorNetManager.hpp"
#include "misc/JSONHelper.hpp"

using namespace std;
using namespace nlohmann;

PhysicalSensorRestApiHandler::PhysicalSensorRestApiHandler(shared_ptr<Logic> _logic)
: RestApiHandler("/physicalSensors"),
  logic(_logic) {
  
}

void PhysicalSensorRestApiHandler::handleScanRequest(struct mg_connection *c) {
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  SensorNetManagerScanResult result = mgr->scanForSensors();
  string status;
  switch(result) {
    case SensorNetManagerScanResult_START:
      status = "started";
      break;

    case SensorNetManagerScanResult_ALREADY_IN_PROGRESS:
      status = "inProgress";
      break;

    default:
    case SensorNetManagerScanResult_FAILED:
      status = "failed";
      break;
  }
  json outJson = {
    {"status", status}
  };
  sendHttpOk(c, outJson.dump());
}

void PhysicalSensorRestApiHandler::handleLastScan(struct mg_connection *c) {

}

void PhysicalSensorRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {

  //Possible commands:
  //none - http://server/physicalSensors - returns sensors in current use (visible for logic)
  //scan - /physicalSensors?cmd=scan - scans for physical sensors
  //lastScan - /physicalSensors?cmd=scan - list all found physical sensors (both visible and invisible for logic)
  string cmd;
  if (getQueryVariable(data, "cmd", &cmd) == true) {
    if (cmd == "scan") {
      handleScanRequest(c);

    } else if (cmd == "lastScan") {
      handleLastScan(c);
    }
    return;
  }

  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  PhysicalSensorList list = make_shared<PhysicalSensorVector>(mgr->getSensors());
  
  json result = toJSON(list);
  string response = result.dump().c_str();
  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s",
            (int) response.length(), response.c_str());
}

void PhysicalSensorRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  shared_ptr<string> str = getBodyOrDie(c, data);
  if (str == nullptr) {
    return;
  }
  string tmp = "[" + *str + "]";
  PhysicalSensorList list = physicalSensorsFromJSON(tmp);
  if (list->size() != 1) {
    badRequest(c);
    return;
  }
  shared_ptr<PhysicalSensor> item = (*list)[0];
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  
  if (mgr->addSensor(item)) {
    mgr->saveConfiguration();
    logic->rebuildListOfMeasurementTasks();
    sendHttpOk(c);
    
  } else {
    conflict(c);
  }
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

