/*
 * AcquisitorTask.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#ifndef AcquisitorTask_hpp
#define AcquisitorTask_hpp

#include "entities/PhysicalSensor.hpp"
#include "misc/LogHelper.hpp"
#include "SensorDataListener.hpp"

using namespace std;

class AcquisitorTask {
  public:
    AcquisitorTask(shared_ptr<PhysicalSensor> sensor, int count, SensorDataListener* listener,
        shared_ptr<spdlog::logger> logger);
    bool execute();
    int getSensorId();
  private:
    shared_ptr<PhysicalSensor>  sensor;
    SensorDataListener* listener;
    int attempts;
    int measurementsCount;
    shared_ptr<spdlog::logger> logger;
};

#endif /* AcquisitorTask_hpp */
