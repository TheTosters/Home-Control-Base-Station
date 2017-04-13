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
  SensorNetManagerStartScanResult result = mgr->scanForSensors();
  string status;
  switch(result) {
    case SensorNetManagerStartScanResult_START:
      status = "started";
      break;

    case SensorNetManagerStartScanResult_ALREADY_IN_PROGRESS:
      status = "inProgress";
      break;

    default:
    case SensorNetManagerStartScanResult_FAILED:
      status = "failed";
      break;
  }
  json outJson = {
    {"status", status}
  };
  sendHttpOk(c, outJson.dump());
}

void PhysicalSensorRestApiHandler::handleLastScan(struct mg_connection *c) {
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  SensorNetManagerScanStatus result = mgr->getCurrentScanStatus();
  string status;
  json list = {};
  switch(result) {
    case SensorNetManagerScanStatus_IN_PROGRESS:
      status = "inProgress";
      break;

    case SensorNetManagerScanStatus_FINISHED_WITH_RESULT:
      status = "done";
      list = toJSON(mgr->getScannedPhysicalSensors());
      break;

    default:
    case SensorNetManagerScanStatus_NO_RESULT:
      status = "empty";
      break;
  }

  json outJson = {
    {"status", status},
    {"sensors", list}
  };
  sendHttpOk(c, outJson.dump());
}

void PhysicalSensorRestApiHandler::handleMeasurements(struct mg_connection *c, void *data) {
  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  PhysicalSensorList list = make_shared<PhysicalSensorVector>(mgr->getSensors());

  string cmd;
  if (getQueryVariable(data, "sensorId", &cmd) == true) {
    const int filterId = atoi(cmd.c_str());
    list->erase(
        std::remove_if(list->begin(), list->end(),
            [=](shared_ptr<PhysicalSensor>& o) { return o->getId() == filterId; }),
        list->end()
    );
  }

  json outJson = measurementsToJSON(list);
  sendHttpOk(c, outJson.dump());
}

void PhysicalSensorRestApiHandler::handleAddScanned(struct mg_connection *c, void *data) {
  long scanId, newId;
  if ((getOrDieQueryVariable(c, data, "scanId", &scanId) == false) ||
      (getOrDieQueryVariable(c, data, "newId", &newId) == false) ){
    return;
  }

  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  PhysicalSensorList list = mgr->getScannedPhysicalSensors();

  //check for newId conflict
  if (mgr->getSensorById(newId) != nullptr) {
    conflict(c);
    return;
  }

  //find scanned sensor
  auto item = std::find_if(list->begin(), list->end(), [=](shared_ptr<PhysicalSensor>& o) {
    return o->getId() == scanId;
  });
  if (item == list->end()) {
    notFound(c);
    return;
  }

  //add sensor
  if (mgr->addSensor(*item)) {
    mgr->saveConfiguration();
    logic->rebuildListOfMeasurementTasks();
    sendHttpOk(c);

  } else {
    conflict(c);
  }
}

void PhysicalSensorRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {

  //Possible commands:
  //none - http://server/physicalSensors - returns sensors in current use (visible for logic), result can be filtered by
  //        ?sensorId=idOfSensor is specified
  //scan - /physicalSensors?cmd=scan - scans for physical sensors
  //lastScan - /physicalSensors?cmd=lastScan - list all found physical sensors (both visible and invisible for logic)
  //addScanned - cmd=addScanned&scanId=xxx&newId=xxx adds to logic sensor which was detected during scan.
  //              scanId = sensor id in scan list, newId -id which will be used in logic after addition
  //measurements - list of last measured values for all sensors, unless &sensorId=idOfSensor is specified
  string cmd;
  if (getQueryVariable(data, "cmd", &cmd) == true) {
    if (cmd == "scan") {
      handleScanRequest(c);

    } else if (cmd == "lastScan") {
      handleLastScan(c);

    } else if (cmd == "measurements") {
      handleMeasurements(c, data);

    } else if (cmd == "addScanned") {
      handleAddScanned(c, data);

    } else {
      badRequest(c);
    }
    return;
  }

  shared_ptr<SensorNetManager> mgr = logic->getSensorsNetManager();
  PhysicalSensorList list = make_shared<PhysicalSensorVector>(mgr->getSensors());
  
  if (getQueryVariable(data, "sensorId", &cmd) == true) {
    const int filterId = atoi(cmd.c_str());
    list->erase(
        std::remove_if(list->begin(), list->end(), [=](shared_ptr<PhysicalSensor>& o) {return o->getId() == filterId;}),
        list->end());
  }

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

