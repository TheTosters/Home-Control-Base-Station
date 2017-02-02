//
//  RoomTemperatureRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 23/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <algorithm>
#include "logic/rules/RoomTemperatureRule.hpp"
#include "SharedStatesConsts.h"
#include "entities/Entities.hpp"

RoomTemperatureRule::RoomTemperatureRule(shared_ptr<Logic> _logic)
: logic(_logic) {
  
}

void RoomTemperatureRule::execute() {
  int wantHeating = (*logic->getSharedState())[STATE_WANT_HEATING];
  if (wantHeating != 0) {
    return;
  }
  
  RoomsList rooms = logic->getRooms();
  shared_ptr<ScheduleMap> heatingPlan = logic->getRoomHeatingPlan();
  for(auto roomIter = rooms->begin(); roomIter != rooms->end(); roomIter++) {
    string name = (*roomIter)->getName();
    shared_ptr<Schedule> schedule = (*heatingPlan)[name];
    if (schedule == nullptr) {
      continue;
    }
    
    bool measured;
    double temp = getTemperatureInRoom(*roomIter, measured);
    if (measured && schedule->getDesiredTemperature() > temp) {
      wantHeating = 1;
      break;
    }
  }
  
  (*logic->getSharedState())[STATE_WANT_HEATING] = wantHeating;
}

double RoomTemperatureRule::getTemperatureInRoom(shared_ptr<Room> room, bool& measured) {
  double result = 0;
  int resultCount = 0;
  auto sensors = room->getSensors();

  for(auto logSensor : *sensors) {
    long physSesnsId = logSensor.getPhysicalSensorId();
    auto phySensor = logic->getSensorsNetManager()->getSensorById(physSesnsId);
    if (phySensor == nullptr) {
      continue;
    }
    
    if (phySensor->isType(TEMPERATURE) == false) {
      continue;
    }
    result += get<1>( *(phySensor->getLastMeasurement(TEMPERATURE)) );
    resultCount++;
  }
  
  return resultCount > 1 ? result / resultCount : result;
}
