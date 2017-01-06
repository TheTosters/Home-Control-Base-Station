//
//  ComunicationLink.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "CommunicationLink.hpp"

CommunicationLink::CommunicationLink(CommunicationLinkType _type, shared_ptr<PhysicalSensor> _device)
:type(_type), device(_device) {
  
}

shared_ptr<string> CommunicationLink::sendCommand(string cmd) {
  return make_shared<string>("VTH(003c,00b0)(013c,01b0)(023c,02b0)VHH(0012b0)AAA(2,2)");
}

shared_ptr<PhysicalSensor>& CommunicationLink::getDevice() {
  return device;
}