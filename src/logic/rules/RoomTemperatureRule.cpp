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

static const string DEFAULT_SCHEDULE_NAME = "all";

RoomTemperatureRule::RoomTemperatureRule(shared_ptr<Logic> logic, string stoveStateName)
: logic(logic), stoveStateName(stoveStateName) {
  
}

void RoomTemperatureRule::execute() {
  int wantHeating = (*logic->getSharedState())[stoveStateName];
  if (wantHeating != 0) {
    return;
  }
  
  RoomsList rooms = logic->getRooms();
  shared_ptr<ScheduleMap> heatingPlan = logic->getRoomHeatingPlan();
  for(auto roomIter = rooms->begin(); roomIter != rooms->end(); roomIter++) {
    string name = (*roomIter)->getName();
    shared_ptr<Schedule> schedule = (*heatingPlan)[name];
    if (schedule == nullptr) {
      //fall back to "universal rules" if defined
      schedule = (*heatingPlan)[DEFAULT_SCHEDULE_NAME];
      if (schedule == nullptr) {
        continue;
      }
    }
    
    bool measured = false;
    double temp = getTemperatureInRoom(*roomIter, measured);
    if (measured && schedule->getDesiredTemperature() < temp) {
      wantHeating = 1;
      break;
    }
  }
  
  (*logic->getSharedState())[stoveStateName] = wantHeating;
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
    
    if (phySensor->isType(PhysicalSensorType_TEMPERATURE) == false) {
      continue;
    }

    auto tmp = phySensor->getLastMeasurement(PhysicalSensorType_TEMPERATURE);
    if (tmp == nullptr) {
      continue;
    }
    result += get<1>( *tmp );
    resultCount++;
  }
  
  measured = resultCount > 0;

  return resultCount > 1 ? result / resultCount : result;
}

void RoomTemperatureRule::setStoveStateName(const string& name) {
  stoveStateName = name;
}
