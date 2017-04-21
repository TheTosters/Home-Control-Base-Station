/*
 * AcquisitorTask.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/AcquisitorTask.hpp>

static const int MAX_RETRY_ATTEMPTS = 10;

AcquisitorTask::AcquisitorTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<spdlog::logger> logger)
: sensor(sensor), logger(logger), attempts(0) {

}

int AcquisitorTask::getSensorId() {
  return sensor->getId();
}

bool AcquisitorTask::execute() {
  printf("[Acquisition task %p] start\n", this);
  attempts++;
  if (innerExecute() == true) {
    logger->info("Task done");
    return true;
  }

  bool notRetry = attempts > MAX_RETRY_ATTEMPTS;
  logger->warn("Acquisition from {}({}) failed, we will retry:{}", sensor->getName(), sensor->getAddress(),
      notRetry ? "NO" : "YES");
  printf("[Acquisition task %p] exit", this);
  return notRetry;
}
