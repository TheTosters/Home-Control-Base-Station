//
//  ComunicationLink.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "sensor-net/CommunicationLink.hpp"
#include <stdlib.h> 
#include <sstream>
#include "bluetooth/BtleCommWrapper.h"

static const int BTLE_TIMEOUT = 6000;

CommunicationLink::CommunicationLink(CommunicationLinkType _type, shared_ptr<PhysicalSensor> _device,
    shared_ptr<spdlog::logger> logger)
:type(_type), device(_device), btleWrapper(nullptr), logger(logger) {
  if (_type == cltBluetooth) {
    btleWrapper = new BtleCommWrapper();
    btleWrapper->connectTo(_device->getAddress(), BTLE_TIMEOUT);
    if (btleWrapper->isConnected()) {
      logger->debug("Connected to {}", _device->getAddress().c_str());
    } else {
      logger->debug("NOT connected to {}", _device->getAddress().c_str());
    }
  }
}

bool CommunicationLink::isConnected() {
  if (btleWrapper != nullptr) {
    return btleWrapper->isConnected();
  }
  return false;
}

CommunicationLink::~CommunicationLink() {
  if (btleWrapper != nullptr) {
    if (btleWrapper->isConnected()) {
      logger->debug("Disconnected from {}",device->getAddress().c_str());
    }
    delete btleWrapper;
  }
}

shared_ptr<string> CommunicationLink::sendCommand(string cmd, bool* success) {
  if (btleWrapper != nullptr) {
    logger->debug("Sending {} to {}", cmd.c_str(), device->getAddress().c_str());
    if (btleWrapper->send(cmd, BTLE_TIMEOUT) == true) {
      string result = btleWrapper->readLine(BTLE_TIMEOUT, success);
      logger->debug("response {}", result.c_str());
      return make_shared<string>(result);
    }
  }
  if (success != nullptr) {
    *success = false;
  }
  return make_shared<string>("");
}

shared_ptr<PhysicalSensor>& CommunicationLink::getDevice() {
  return device;
}
