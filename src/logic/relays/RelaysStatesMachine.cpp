/*
 * RelaysStatesMachine.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#include <logic/relays/RelaysStatesMachine.hpp>
#include <algorithm>

RelaysStatesMachine::RelaysStatesMachine(shared_ptr<spdlog::logger> logger, shared_ptr<SensorNetManager> sensorNetManager)
: logger(logger), sensorNetManager(sensorNetManager){
}

void RelaysStatesMachine::setConfigFile(const string& path) {
  this->configPath = path;
}

void RelaysStatesMachine::addRelay(int relayId, string name, shared_ptr<PhysicalSensor> relayDevice, int onDeviceIndex,
    bool defaultState) {

  auto iter = find_if(relays.begin(), relays.end(), [relayId](shared_ptr<RelayState>& item){
    return item->getRelayId() == relayId;
  } );

  if (iter != relays.end()) {
    logger->error("Can't add relay with id {}, this id is already used.", relayId);
    return;
  }

  logger->info("Adding new relay with id:{}, using device:{}({}), on device index:{}, default state:{}",
      relayId, relayDevice->getName(), relayDevice->getAddress(), onDeviceIndex, defaultState);
  auto relay = make_shared<RelayState>(relayId, name, relayDevice, onDeviceIndex, defaultState, logger);
  relays.push_back(relay);
}

void RelaysStatesMachine::removeRelay(int relayId) {
  auto iter = remove_if(relays.begin(), relays.end(), [relayId](shared_ptr<RelayState>& item){
    return item->getRelayId() == relayId;
  } );

  relays.erase(iter);
}

shared_ptr<RelayState> RelaysStatesMachine::getRelayState(int id) {
  auto iter = find_if(relays.begin(), relays.end(), [id](shared_ptr<RelayState>& item) {
    return item->getRelayId() == id;
  });

  if (iter != relays.end()) {
    logger->error("Can't find relay with id {}.", id);
    return nullptr;

  } else {
    return *iter;
  }
}

void RelaysStatesMachine::demandRelayState(int relayId, bool relayState, long long duration) {
  auto iter = find_if(relays.begin(), relays.end(), [relayId](shared_ptr<RelayState>& item) {
    return item->getRelayId() == relayId;
  });

  if (iter == relays.end()) {
    logger->error("Request to change unknown relay with id:{}", relayId);
    return;
  }

  shared_ptr<RelayState> innerState = *iter;
  if (innerState->getRequestedState() == relayState) {
    return;
  }
  auto sensor = innerState->getPhysicalSensor();
  logger->error("Request to change relay id:{} to state:{}", relayId, relayState);

  //request state
  int cmdId = sensorNetManager->sendRelayState(sensor, innerState->getRelayIndex(), relayState, duration, innerState);
  innerState->setRequestedState(cmdId, relayState, duration);

  logger->debug("Request relay id {} state change to {} ({}s) -> sensor{}({}) relay index:{}", relayId, relayState,
      duration, sensor->getName(), sensor->getAddress(), innerState->getRelayIndex());
}
