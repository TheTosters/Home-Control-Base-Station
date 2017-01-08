//
//  ComunicationLink.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "CommunicationLink.hpp"
#include <stdlib.h> 
#include <sstream>

CommunicationLink::CommunicationLink(CommunicationLinkType _type, shared_ptr<PhysicalSensor> _device)
:type(_type), device(_device) {
  
}

shared_ptr<string> CommunicationLink::sendCommand(string cmd) {
  int timeOffset = rand() % 30;
  int value = rand() % 100;
  stringstream tmp;
  if (device->getAddress() == "aa:ff:22:99") {
    tmp << "VTH(" << hex << timeOffset << "," << value << ")";
    
  } else if (device->getAddress() == "bb:ff:22:99") {
    tmp << "VHH(" << hex << timeOffset << "," << value << ")";
    
  } else if (device->getAddress() == "cc:ff:22:99") {
    tmp << "VPH(" << hex << timeOffset << "," << value << ")";
    
  } else {
    tmp << "VTH(" << hex << timeOffset << "," << value << ")";
    value = rand() % 100;
    tmp << "VHH(" << hex << timeOffset << "," << value << ")";
  }
  
  return make_shared<string>(tmp.str());
}

shared_ptr<PhysicalSensor>& CommunicationLink::getDevice() {
  return device;
}