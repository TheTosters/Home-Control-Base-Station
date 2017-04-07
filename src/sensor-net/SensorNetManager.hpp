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

class SensorNetManager : public HciWrapperListener {
  public:
    SensorNetManager();
    virtual ~SensorNetManager();
    PhysicalSensorVector getSensors();
    void setSensorsList(PhysicalSensorList sensorsList);
    MeasurementMap fetchMeasurements(shared_ptr<PhysicalSensor> sensor, int count = 1);
    void saveConfiguration();
    bool deleteSensor(long sensorId);
    bool addSensor(shared_ptr<PhysicalSensor> sensor);
    shared_ptr<PhysicalSensor> getSensorById(long id);
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

    void resolverThreadMain();
    void scanningThreadMain();
    void finalizeScanningThread();
  protected:
    virtual void onScanStart() override;
    virtual void onScanStop() override;
    virtual void onNewDeviceFound(const BTLEDevice& device) override;
};
#endif /* SensorNetManager_hpp */
