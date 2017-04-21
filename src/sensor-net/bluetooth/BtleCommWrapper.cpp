/*
 * BtleCommWrapper.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Zarnowski
 */

#include "BtleCommWrapper.h"
#include <algorithm>
#include <sys/time.h>
#include "unistd.h"

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}

BtleCommWrapper::BtleCommWrapper()
: charToUse(nullptr), connected(false), readDone(false), writeDone(false) {

}

BtleCommWrapper::~BtleCommWrapper() {
  btle.close();
}

void BtleCommWrapper::installCallbacks() {
  btle.cb_get_client_characteristic_configuration = [=] {
    onFoundServicesAndCharacteristics();
  };
  btle.cb_disconnected = [=] (BLEGATTStateMachine::Disconnect d) {
    onDisconnected(d);
  };
  btle.cb_connected = [=] {
    onConnected();
  };
  btle.cb_services_read = [=] {
    btle.find_all_characteristics();
  };
  btle.cb_find_characteristics = [=] {
    btle.get_client_characteristic_configuration();
  };
  btle.cb_write_response = [=] {
    writeDone = true;
  };
}

bool BtleCommWrapper::connectTo(const string& address, int timeoutInMs) {
  installCallbacks();
  notificationBuffer.clear();
  const long long startTime = current_timestamp();
  try {
    btle.connect_blocking(address);
    while (connected && charToUse == nullptr) {
      btle.read_and_process_next();
      if (current_timestamp() - startTime > timeoutInMs) {
        printf("Connection timeout\n");
        break;
      }
      usleep(50 * 1000);
    }
  } catch (...) {
    connected = false;
    return false;
  }
  return isConnected();
}

bool BtleCommWrapper::isConnected() {
  return connected && charToUse != nullptr;
}

void BtleCommWrapper::disconnect() {
  btle.close();
}

bool BtleCommWrapper::send(const string& data, int timeoutInMs) {
  if (isConnected() == false) {
    return false;
  }
  writeDone = false;
  const long long startTime = current_timestamp();
  try {
    charToUse->write_request((const uint8_t*) data.c_str(), data.length());
    while (isConnected() && writeDone == false) {
      //btle.write_and_process_next();
      btle.read_and_process_next();
      if (current_timestamp() - startTime > timeoutInMs) {
        printf("Connection timeout\n");
        break;
      }
      usleep(50 * 1000);
    }
  } catch(...) {
    printf("Exception at send\n");
  }

  return isConnected() && writeDone;
}

bool BtleCommWrapper::readLineFromBuffer(string& result) {
  if (notificationBuffer.size() == 0) {
    return false;
  }
  std::vector<uint8_t>::iterator it = std::find_if(notificationBuffer.begin(), notificationBuffer.end(),
      [](int i){ return i == 13;});

  if (it == notificationBuffer.end()) {
    return false;
  }

  it++;
  uint8_t* startAddress = &(notificationBuffer[0]);
  int size = it - notificationBuffer.begin();
  if (size < 0) {
    printf("%s Overflow!\n", __func__);
    return false;
  }
  while (size > 0 && startAddress[size - 1] == '\r') {
    size--;
  }
  if (size > 0) {
    result.append((const char*) startAddress, size);
    notificationBuffer.erase(notificationBuffer.begin(), it);
  }

  return true;
}

string BtleCommWrapper::readLine(int timeoutInMs) {
  if (isConnected() == false) {
    printf("readLine canceled, not connected\n");
    return "";
  }

  const long long startTime = current_timestamp();

  string result = "";
  try{
    while (isConnected() && readDone == false) {
      printf(">>\n");
      btle.read_and_process_next();
      printf("<<\n");
      if (current_timestamp() - startTime > timeoutInMs) {
        printf("readLine timeout\n");
        break;
      }

      //break if whole line is present, otherwise continue reading
      if (readLineFromBuffer(result) == false) {
        readDone = false;
      }
      usleep(50 * 1000);
    }
  } catch (...) {
    printf("exception at read\n");
  }
  return result;
}

void BtleCommWrapper::onFoundServicesAndCharacteristics() {

  for (auto& service : btle.primary_services) {
    for (auto& characteristic : service.characteristics) {
      if (characteristic.uuid == UUID("ffe1")) {
        charToUse = &characteristic;
        characteristic.cb_notify_or_indicate = [=](const PDUNotificationOrIndication& n){
          onCharacteristicNotification(n);
        };
        characteristic.set_notify_and_indicate(true, false);
        return; //no need to search more
      }
    }
  }
}

void BtleCommWrapper::onDisconnected(BLEGATTStateMachine::Disconnect d) {
  charToUse = nullptr;
  connected = false;
}

void BtleCommWrapper::onCharacteristicNotification(const PDUNotificationOrIndication& notif) {
  uint8_t* data = (uint8_t*) notif.value().first;
  int len = ((uint8_t*) notif.value().second - data);
  for(;len > 0; len--) {
    notificationBuffer.push_back(*data);
    data++;
  }
  readDone = true;
}

void BtleCommWrapper::onConnected() {
  connected = true;
  btle.read_primary_services();
}
