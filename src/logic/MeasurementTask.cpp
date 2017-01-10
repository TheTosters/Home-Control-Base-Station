//
//  MeasurementTask.cpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "MeasurementTask.hpp"

MeasurementTask::MeasurementTask(shared_ptr<PhysicalSensor> sens)
: nextMeasurementTime(sens->getDesiredFetchDelay() + time(nullptr)), sensor(sens) {
  
}

bool operator< (const shared_ptr<MeasurementTask>& lhs, const shared_ptr<MeasurementTask>& rhs){
  return rhs->getNextMeasurementTime() < lhs->getNextMeasurementTime();
}

bool MeasurementTask::operator<(const MeasurementTask& other) const {
  return other.nextMeasurementTime < nextMeasurementTime;
}

bool MeasurementTask::operator<(const shared_ptr<MeasurementTask>& other) const {
  return other->nextMeasurementTime < nextMeasurementTime;
}

time_t MeasurementTask::getNextMeasurementTime() const {
  return nextMeasurementTime;
}

shared_ptr<PhysicalSensor> MeasurementTask::getSensor() const {
  return sensor;
}

time_t MeasurementTask::getTimeToMeasure() {
  time_t now = time(nullptr);
  
  return (nextMeasurementTime <= now) ? 0 : (nextMeasurementTime - now);
}