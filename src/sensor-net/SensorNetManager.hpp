//
//  SensorNetManager.hpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SensorNetManager_hpp
#define SensorNetManager_hpp

#include <stdio.h>
#include <thread>
#include <vector>
#include <mutex>
#include "entities/PhysicalSensor.hpp"
#include "json/json.hpp"
#include "sensor-net/SensorNetProtocolParser.hpp"
#include "misc/LogHelper.hpp"
#include "bluetooth/HciWrapper.hpp"
#include "sensor-net/SensorDataListener.hpp"
#include <misc/SimpleActionListener.hpp>

using namespace std;
using namespace nlohmann;

enum SensorNetManagerStartScanResult {
  SensorNetManagerStartScanResult_START,
  SensorNetManagerStartScanResult_ALREADY_IN_PROGRESS,
  SensorNetManagerStartScanResult_FAILED
};

enum SensorNetManagerScanStatus {
  SensorNetManagerScanStatus_NO_RESULT,
  SensorNetManagerScanStatus_IN_PROGRESS,
  SensorNetManagerScanStatus_FINISHED_WITH_RESULT
};

class PhysicalSensor;
class DataAcquisitor;

class SensorNetManager : public HciWrapperListener {
  public:
    SensorNetManager();
    virtual ~SensorNetManager();

    void fetchMeasurements(shared_ptr<PhysicalSensor> sensor, SensorDataListener* listener, int count = 1);
    int sendRelayState(shared_ptr<PhysicalSensor> sensor, int relayIndex, bool turnedOn, int duration,
        shared_ptr<SimpleActionListener> listener);

    void setSensorsConfigFile(const string& filename);
    void saveConfiguration();

    bool addSensor(shared_ptr<PhysicalSensor> sensor);
    bool deleteSensor(long sensorId);
    PhysicalSensorVector getSensors();
    shared_ptr<PhysicalSensor> getSensorById(long id);
    void setSensorsList(PhysicalSensorList sensorsList);

    SensorNetManagerStartScanResult scanForSensors();
    SensorNetManagerScanStatus getCurrentScanStatus();
    PhysicalSensorList getScannedPhysicalSensors();
  private:
    PhysicalSensorList    sensors;
    mutex                 managerMutex;
    shared_ptr<spdlog::logger> logger;
    PhysicalSensorList    scannedSensors;
    vector<BTLEDevice>    scannedDevices;
    HciWrapper*           hciWrapper;
    thread*               resolverThread;
    thread*               scanningThread;
    int                   nextScanId;
    string                sensorsConfigFile;
    DataAcquisitor*       acquisitor;

    void resolverThreadMain();
    void scanningThreadMain();
    void finalizeScanningThread();
  protected:
    virtual void onScanStart() override;
    virtual void onScanStop() override;
    virtual void onNewDeviceFound(const BTLEDevice& device) override;
    bool probeSensor(shared_ptr<PhysicalSensor> sensor, int& startingStep, bool& shouldRetry);
};
#endif /* SensorNetManager_hpp */
