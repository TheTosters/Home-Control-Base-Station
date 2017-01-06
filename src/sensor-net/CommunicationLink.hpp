//
//  ComunicationLink.hpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef ComunicationLink_hpp
#define ComunicationLink_hpp

#include <stdio.h>
#include <string>
#include "PhysicalSensor.hpp"

using namespace std;

enum CommunicationLinkType {
  cltBluetooth
};

class CommunicationLink {
  public:
    CommunicationLink(CommunicationLinkType type, shared_ptr<PhysicalSensor> device);
  
    shared_ptr<string> sendCommand(string cmd);
    shared_ptr<PhysicalSensor>& getDevice();
  private:
    CommunicationLinkType       type;
    shared_ptr<PhysicalSensor>  device;
};

#endif /* ComunicationLink_hpp */
