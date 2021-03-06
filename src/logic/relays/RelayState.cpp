/*
 * RelayState.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#include <logic/relays/RelayState.hpp>
#include <misc/Helper.h>
#include <algorithm>

//this is time which is subtracted from duration after command is delivered
static const long long SAFETY_TIME_DELTA = 3000;  //in ms

RelayStateSentCommand::RelayStateSentCommand(int commandId, bool requestedState, long long durationOfState,
        RelayStateSentCommandType type, CommandDelivereedRelayState commandDeliveredState)
: commandId(commandId), requestedState(requestedState), durationOfState(durationOfState),
  type(type), commandDeliveredState(commandDeliveredState) {

}

RelayState::RelayState(int relayId, string name, shared_ptr<PhysicalSensor> sensor, int sensorRelayIndex,
    bool initialState, shared_ptr<spdlog::logger> logger)
: relayId(relayId),
  sensor(sensor),
  sensorRelayIndex(sensorRelayIndex),
  timeOfChange(0),
  initialState(initialState),
  requestedState(false),
  durationOfState(0),
  logger(logger),
  name(name) {

}

void RelayState::setInitialState(int commandId, bool state) {
  //NOPE: initialState = state;
  //send request to node, and if this is valid then remember it :)
  commandsToTrace.push_back(RelayStateSentCommand(commandId, state, 0, rssctChangeInitialState, cdrsWaiting));
}

bool RelayState::getInitialState() {
  return initialState;
}

bool RelayState::isRelayEnabled() {
  if (durationOfState > 0) {
    if (Helper::currentTimestamp() > timeOfChange + durationOfState) {
      return initialState;

    } else {
      return requestedState;
    }

  } else {
    return initialState;
  }
}

long long RelayState::timeToStateChange() {
  if (durationOfState > 0) {
    return Helper::currentTimestamp() - (timeOfChange + durationOfState);

  } else {
    return -1;
  }
}

void RelayState::setRequestedState(int commandId, bool requestedState, long long duration) {
  commandsToTrace.push_back(RelayStateSentCommand(commandId, requestedState, duration, rssctRequestState, cdrsWaiting));
}

bool RelayState::getRequestedState() {
  bool result = isRelayEnabled();
  for(auto iter = commandsToTrace.begin(); iter != commandsToTrace.end(); iter++) {
    if ( (*iter).type == rssctRequestState ) {
      result = (*iter).requestedState;
    }
  }

  return result;
}

void RelayState::applyCommand(RelayStateSentCommand& cmd) {
  if (cmd.type == rssctRequestState) {
    timeOfChange = Helper::currentTimestamp();
    requestedState = cmd.requestedState;
    durationOfState = cmd.durationOfState - SAFETY_TIME_DELTA;

  } else if (cmd.type == rssctChangeInitialState) {
    initialState = cmd.requestedState;

  } else {
    //this is bad, and I'm sad
    logger->error("Unknown command id:{} type:{}", cmd.commandId, cmd.type);
  }
}

void RelayState::onActionSuccess(int id, Number result) {
  logger->debug("Action id:{}, succeed.", id);
  auto iter = std::find_if(commandsToTrace.begin(), commandsToTrace.end(), [id](RelayStateSentCommand& cmd){
    return cmd.commandId == id;
  });

  if (iter != commandsToTrace.end()) {
    applyCommand(*iter);
    commandsToTrace.erase(iter);
  }
}

void RelayState::onActionError(int id, int error) {
  logger->warn("Action id:{}, failed with error code:{}", id, error);
  onActionFailure(id);
}

void RelayState::onActionFailure(int id) {
  //just drop this command
  logger->warn("Action id:{}, failed.", id);
  auto iter = std::remove_if(commandsToTrace.begin(), commandsToTrace.end(), [id](RelayStateSentCommand& cmd){
    return cmd.commandId == id;
  });
  commandsToTrace.erase(iter, commandsToTrace.end());
}

int RelayState::getRelayId() {
  return relayId;
}

void RelayState::setName(const string& name) {
  this->name = name;
}

string RelayState::getName() {
  return name;
}

shared_ptr<PhysicalSensor> RelayState::getPhysicalSensor() {
  return sensor;
}

int RelayState::getRelayIndex() {
  return sensorRelayIndex;
}
