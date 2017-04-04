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

static const int BTLE_TIMEOUT = 3000;

CommunicationLink::CommunicationLink(CommunicationLinkType _type, shared_ptr<PhysicalSensor> _device)
:type(_type), device(_device), btleWrapper(nullptr) {
  if (_type == cltBluetooth) {
    btleWrapper = new BtleCommWrapper();
    btleWrapper->connectTo(_device->getAddress());
  }
}

CommunicationLink::~CommunicationLink() {
  if (btleWrapper != nullptr) {
    delete btleWrapper;
  }
}

shared_ptr<string> CommunicationLink::sendCommand(string cmd) {
  if (btleWrapper != nullptr) {
    btleWrapper->send(cmd, BTLE_TIMEOUT);
    string result = btleWrapper->readLine(BTLE_TIMEOUT, true);
    return make_shared<string>(result);
  }
  return make_shared<string>("");
}

shared_ptr<PhysicalSensor>& CommunicationLink::getDevice() {
  return device;
}
