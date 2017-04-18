/*
 * DataAcquisitor.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#ifndef DataAcquisitor_hpp
#define DataAcquisitor_hpp

#include <thread>
#include <vector>
#include <mutex>
#include "entities/PhysicalSensor.hpp"
#include "misc/LogHelper.hpp"
#include "SensorDataListener.hpp"
#include "sensor-net/AcquisitorTask.hpp"

using namespace std;

class DataAcquisitor {
  public:
    DataAcquisitor(shared_ptr<spdlog::logger> logger);
    virtual ~DataAcquisitor();

    void fetch(shared_ptr<PhysicalSensor> sensor, shared_ptr<SensorDataListener> listener);

    void pause(bool waitForPause = true);
    bool isPaused();
    void resume();
  private:
    thread* workerThread;
    mutex   innerMutex;
    shared_ptr<spdlog::logger> logger;
    bool paused;

    vector<shared_ptr<AcquisitorTask>>  tasksToExecute;
    void stopThread();
    void startThread();

    void workerThreadMain();
};

#endif /* DataAcquisitor_hpp */