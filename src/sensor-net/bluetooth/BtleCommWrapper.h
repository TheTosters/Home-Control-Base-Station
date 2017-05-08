/*
 * BtleCommWrapper.h
 *
 *  Created on: Apr 20, 2017
 *      Author: Zarnowski
 */

#ifndef BtleCommWrapper_hpp
#define BtleCommWrapper_hpp

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include "blepp/blepp/blestatemachine.h"
#include <sstream>

using namespace std;
using namespace BLEPP;

class BtleCommWrapper {
  public:
    BtleCommWrapper();
    virtual ~BtleCommWrapper();

    bool connectTo(const string& address, int timeoutInMs = 6000);
    bool isConnected();
    void disconnect();
    bool send(const string& data, int timeoutInMs = 6000);
    string readLine(int timeoutInMs = 6000, bool* error = nullptr);
  private:
    BLEGATTStateMachine btle;
    Characteristic* charToUse;
    bool connected;
    bool readDone;
    bool writeDone;
    std::vector<uint8_t> notificationBuffer;

    void onFoundServicesAndCharacteristics();
    void onDisconnected(BLEGATTStateMachine::Disconnect d);
    void onCharacteristicNotification(const PDUNotificationOrIndication& notif);
    void onConnected();
    bool readLineFromBuffer(string& result);
    void installCallbacks();
};

#endif /* BtleCommWrapper_hpp */
