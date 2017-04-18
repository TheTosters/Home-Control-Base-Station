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
    AcquisitorTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<SensorDataListener> listener);
    void execute();
    bool shouldReschedule();
    int getSensorId();
  private:
    shared_ptr<PhysicalSensor>  sensor;
    shared_ptr<SensorDataListener> listener;
    int attempts;
    MeasurementMap measurements;
};

#endif /* AcquisitorTask_hpp */
