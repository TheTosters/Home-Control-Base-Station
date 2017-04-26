/*
 * SendSimpleCommandTask.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/SendSimpleCommandTask.hpp>

SendSimpleCommandTask::SendSimpleCommandTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<string> command,
    NumbersList argList, shared_ptr<SimpleActionListener> listener, shared_ptr<spdlog::logger> logger)
: AcquisitorTask(sensor, logger), command(command), argList(argList), listener(listener) {

}

bool SendSimpleCommandTask::innerExecute() {
//TODO: dokonczyc -> SensorNetProtocolParser::sendSimpleCommand
  if (listener) {
    listener->onActionError(taskId, 0);
  } else {
    listener->onActionSuccess(taskId);
  }
}

void SendSimpleCommandTask::giveUp() {
  if (listener) {
    listener->onActionFailure(taskId);
  }
}
