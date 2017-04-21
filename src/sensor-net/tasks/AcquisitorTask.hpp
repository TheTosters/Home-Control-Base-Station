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
#include "sensor-net/SensorDataListener.hpp"

using namespace std;

class AcquisitorTask {
  public:
    AcquisitorTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<spdlog::logger> logger);
    virtual ~AcquisitorTask() = default;

    bool execute();
    int getSensorId();
    int getTaskId();
  protected:
    shared_ptr<PhysicalSensor>  sensor;
    shared_ptr<spdlog::logger> logger;
    int attempts;
    const int taskId;

    virtual bool innerExecute() = 0;
    virtual void giveUp();
};

#endif /* AcquisitorTask_hpp */
