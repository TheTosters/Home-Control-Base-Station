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
#include "misc/LogHelper.hpp"

using namespace std;

enum CommunicationLinkType {
  cltBluetooth
};

class BtleCommWrapper;

class CommunicationLink {
  public:
    CommunicationLink(CommunicationLinkType type, shared_ptr<PhysicalSensor> device, shared_ptr<spdlog::logger> logger);
    virtual ~CommunicationLink();

    shared_ptr<string> sendCommand(string cmd);
    shared_ptr<PhysicalSensor>& getDevice();
    bool isConnected();
  private:
    CommunicationLinkType       type;
    shared_ptr<PhysicalSensor>  device;
    BtleCommWrapper*            btleWrapper;
    shared_ptr<spdlog::logger> logger;
};

#endif /* ComunicationLink_hpp */
