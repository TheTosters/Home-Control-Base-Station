/*
 * RelaysStatesMachine.h
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#ifndef RelaysStatesMachine_hpp
#define RelaysStatesMachine_hpp

#include <memory>
#include <vector>
#include "RelayState.hpp"
#include <entities/PhysicalSensor.hpp>
#include <misc/LogHelper.hpp>

using namespace std;

class RelaysStatesMachine {
  public:
    RelaysStatesMachine(shared_ptr<spdlog::logger> logger);
    virtual ~RelaysStatesMachine() = default;

    void addRelay(int relayId, string name, shared_ptr<PhysicalSensor> relayDevice, int onDeviceIndex, bool defaultState);
    void removeRelay(int relayId);
    shared_ptr<RelayState> getRelayState(int id);
    void setConfigFile(const string& path);
  private:
    shared_ptr<spdlog::logger> logger;
    vector<shared_ptr<RelayState>> relays;
    string configPath;
};

#endif /* RelaysStatesMachine_hpp */
