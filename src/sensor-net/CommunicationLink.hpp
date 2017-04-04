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
#include "entities/PhysicalSensor.hpp"

using namespace std;

enum CommunicationLinkType {
  cltBluetooth
};

class BtleCommWrapper;

class CommunicationLink {
  public:
    CommunicationLink(CommunicationLinkType type, shared_ptr<PhysicalSensor> device);
    virtual ~CommunicationLink();

    shared_ptr<string> sendCommand(string cmd);
    shared_ptr<PhysicalSensor>& getDevice();
  private:
    CommunicationLinkType       type;
    shared_ptr<PhysicalSensor>  device;
    BtleCommWrapper*            btleWrapper;
};

#endif /* ComunicationLink_hpp */
