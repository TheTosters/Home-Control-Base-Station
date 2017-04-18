/*
 * SensorDataListener.hpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#ifndef SensorDataListener_hpp
#define SensorDataListener_hpp

#include "entities/PhysicalSensor.hpp"
#include "entities/SensorValue.hpp"

using namespace std;

class SensorDataListener {
  public:
    virtual ~SensorDataListener() = default;
    virtual void onSensorData(shared_ptr<PhysicalSensor>, MeasurementMap measurements) = 0;
};

#endif /* SensorDataListener_hpp */
