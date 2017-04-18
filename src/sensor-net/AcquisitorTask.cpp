/*
 * AcquisitorTask.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/AcquisitorTask.hpp>

static const int MAX_RETRY_ATTEMPTS = 10;

AcquisitorTask::AcquisitorTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<SensorDataListener> listener)
: sensor(sensor), listener(listener), attempts(0) {

}

int AcquisitorTask::getSensorId() {
  return sensor->getId();
}

void AcquisitorTask::execute(){

}

bool AcquisitorTask::shouldReschedule() {

}
