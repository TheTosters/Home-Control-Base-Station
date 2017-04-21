/*
 * AcquisitorTask.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/AcquisitorTask.hpp>

static const int MAX_RETRY_ATTEMPTS = 10;
static int nextTaskId = 0;

AcquisitorTask::AcquisitorTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<spdlog::logger> logger)
: sensor(sensor), logger(logger), attempts(0), taskId(++nextTaskId) {

}

int AcquisitorTask::getSensorId() {
  return sensor->getId();
}

int AcquisitorTask::getTaskId() {
  return taskId;
}

bool AcquisitorTask::execute() {

  logger->info("Acquisition task {} begin", taskId);
  attempts++;
  if (innerExecute() == true) {
    logger->info("Task {} succeed", taskId);
    return true;
  }

  bool notRetry = attempts > MAX_RETRY_ATTEMPTS;
  if (notRetry == true) {
    giveUp();
  }
  logger->warn("Acquisition task {} from {}({}) failed, we will retry:{}", taskId, sensor->getName(),
      sensor->getAddress(), notRetry ? "NO" : "YES");
  return notRetry;
}

void AcquisitorTask::giveUp() {
  //we will not try to execute again...
  logger->warn("Giving up with task id {}", taskId);
}
