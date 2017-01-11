//
//  MeasurementsRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 11/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <time.h>
#include "MeasurementsRestApiHandler.hpp"

#include "json.hpp"
#include "SensorNetManager.hpp"
#include "JSONHelper.hpp"
#include "SimpleCriteria.hpp"
#include "Storage.hpp"
#include "SQLiteSensorValueSerializer.hpp"

using namespace std;
using namespace nlohmann;

MeasurementsRestApiHandler::MeasurementsRestApiHandler(shared_ptr<Logic> _logic)
: RestApiHandler("/measurements"),
  logic(_logic) {
  
}

void MeasurementsRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  //two kinds of requests:
  //cmd=get, sensorId, offset, count, resultType[optional], from[optional, timestamp], to[optional, timestamp]
  //cmd=info, sensorId, resultType[optional], from[optional, timestamp], to[optional, timestamp]
  
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
