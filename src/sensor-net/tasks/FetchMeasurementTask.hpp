/*
 * FetchMeasurementTask.h
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#ifndef FetchMeasurementTask_hpp
#define FetchMeasurementTask_hpp

#include <sensor-net/tasks/AcquisitorTask.hpp>

class FetchMeasurementTask: public AcquisitorTask {
  public:
    FetchMeasurementTask(shared_ptr<PhysicalSensor> sensor, int count, SensorDataListener* listener,
        shared_ptr<spdlog::logger> logger);
    virtual ~FetchMeasurementTask() = default;

  protected:
    SensorDataListener* listener;
    int measurementsCount;

    virtual bool innerExecute() override;
};

#endif /* FetchMeasurementTask_hpp */
