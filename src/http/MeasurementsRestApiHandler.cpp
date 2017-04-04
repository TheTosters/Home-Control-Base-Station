//
//  MeasurementsRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 11/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <time.h>
#include "http/MeasurementsRestApiHandler.hpp"

#include "json/json.hpp"
#include "sensor-net/SensorNetManager.hpp"
#include "misc/JSONHelper.hpp"
#include "storage/SimpleCriteria.hpp"
#include "storage/Storage.hpp"
#include "storage/sqlite/SQLiteSensorValueSerializer.hpp"

using namespace std;
using namespace nlohmann;

MeasurementsRestApiHandler::MeasurementsRestApiHandler(shared_ptr<Logic> _logic)
: RestApiHandler("/measurements"),
  logic(_logic) {
  
}

void MeasurementsRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  //two kinds of requests:
  //cmd=get, sensorId, offset[=0], count[=30], resultType[optional], from[optional, timestamp], to[optional, timestamp]
  //cmd=info, sensorId, resultType[optional], from[optional, timestamp], to[optional, timestamp]
  string cmd;
  if (getOrDieQueryVariable(c, data, "cmd", &cmd) == false) {
    return;
  }
  
  SimpleCriteria criteria;
  parseCriteria(data, criteria, true);
  
  if (criteria.helperId < 0) {
    missingQueryVariable(c, "sensorId");
    return;
  }
  
  if (cmd == "get") {
    criteria.offset = criteria.offset < 0 ? 0 : criteria.offset;
    criteria.count = criteria.count < 0 ? 30 : criteria.count;
    criteria.count = criteria.count > 100 ? 100 : criteria.count;
    auto serialiser = logic->getStorage()->requestSerializer<SQLiteSensorValueSerializer>(SensorValue());
    auto result = serialiser->loadMatching(criteria);
    json outJson = toJSON(result, false);
    sendHttpOk(c, outJson.dump());

  } else if (cmd == "info") {
    auto serialiser = logic->getStorage()->requestSerializer<SQLiteSensorValueSerializer>(SensorValue());
    long result = serialiser->count(criteria);
    json outJson = {
      "count", result
    };
    sendHttpOk(c, outJson.dump());
    
  } else {
    badRequest(c);
  }
}

void MeasurementsRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  methodNotSupported(c, "GET, DELETE");
}

void MeasurementsRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  //two kinds of requests:
  //sensorId, resultType[optional], from[optional, timestamp], to[optional, timestamp]
  //measurementId
  
  SimpleCriteria criteria;
  if (getQueryVariable(data, "sensorId", &criteria.helperId)) {
    parseCriteria(data, criteria, false);
    
  } else if (getQueryVariable(data, "measurementId", &criteria.id)) {
  
  } else {
    
    badRequest(c);
    return;
  }
  
  auto serialiser = logic->getStorage()->requestSerializer<SQLiteSensorValueSerializer>(SensorValue());
  if (serialiser->remove(criteria)) {
    sendHttpOk(c);
    
  } else {
    internalError(c);
  }
}

void MeasurementsRestApiHandler::parseCriteria(void *data, SimpleCriteria& criteria, bool parseNavi) {
  if (getQueryVariable(data, "sensorId", &criteria.helperId) == false) {
    criteria.helperId = -1;
  }
  
  if (getQueryVariable(data, "resultType", &criteria.type) == false) {
    criteria.type = -1;
  }
  
  long tmp;
  if (getQueryVariable(data, "from", &tmp) == true) {
    criteria.from = static_cast<time_t>(tmp);
  }
  
  if (getQueryVariable(data, "to", &tmp) == true) {
    criteria.to = static_cast<time_t>(tmp);
  }
  
  if (parseNavi) {
    if (getQueryVariable(data, "offset", &criteria.offset) == false) {
      criteria.offset = -1;
    }
    
    if (getQueryVariable(data, "count", &criteria.count) == false) {
      criteria.count = -1;
    }
  }
}
