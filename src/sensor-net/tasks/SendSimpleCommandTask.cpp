/*
 * SendSimpleCommandTask.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/SendSimpleCommandTask.hpp>

SendSimpleCommandTask::SendSimpleCommandTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<spdlog::logger> logger)
: AcquisitorTask(sensor, logger) {

}

bool SendSimpleCommandTask::innerExecute() {
}

void SendSimpleCommandTask::giveUp() {
  //todo: notify listener
}
