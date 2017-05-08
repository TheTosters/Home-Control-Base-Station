/*
 * SendSimpleCommandTask.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/SendSimpleCommandTask.hpp>
#include <sensor-net/SensorNetProtocolParser.hpp>
#include <sensor-net/CommunicationLink.hpp>
#include <sensor-net/parsers/Number.hpp>

SendSimpleCommandTask::SendSimpleCommandTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<string> command,
    NumbersList argList, shared_ptr<SimpleActionListener> listener, shared_ptr<spdlog::logger> logger)
: AcquisitorTask(sensor, logger), command(command), argList(argList), listener(listener) {

}

bool SendSimpleCommandTask::innerExecute() {
//TODO: dokonczyc -> SensorNetProtocolParser::sendSimpleCommand
  try {
    CommunicationLink link(cltBluetooth, sensor, logger);
    if (link.isConnected()) {
      SensorNetProtocolParser parser(&link);
      bool error = false;
      Number result = parser.sendSimpleCommand(command, argList, &error);

      if (listener) {
        if (error == true) {
          listener->onActionError(taskId, result.asInt());

        } else {
          listener->onActionSuccess(taskId, result);
        }
      }
      return true;
    }

  } catch (std::exception const& e) {
    logger->error("Exception at {}", __func__);
    //logger->error(e.what());  //hmm this cause thread crash :(

  } catch (...) {
    logger->error("Undefined exception at {}", __func__);
  }

  if (listener) {
    listener->onActionError(taskId, -1);
  }
  return false;
}

void SendSimpleCommandTask::giveUp() {
  if (listener) {
    listener->onActionFailure(taskId);
  }
}
