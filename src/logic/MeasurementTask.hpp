//
//  MeasurementTask.hpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef MeasurementTask_hpp
#define MeasurementTask_hpp

#include <stdio.h>
#include <time.h>
#include "entities/PhysicalSensor.hpp"

class MeasurementTask {
  public:
    MeasurementTask(shared_ptr<PhysicalSensor> sensor);
  
    bool operator<(const MeasurementTask& other) const;
    bool operator<(const shared_ptr<MeasurementTask>& other) const;
    time_t  getNextMeasurementTime() const;
    shared_ptr<PhysicalSensor> getSensor() const;
    time_t  getTimeToMeasure();
  private:
    time_t                      nextMeasurementTime;
    shared_ptr<PhysicalSensor>  sensor;
};

bool operator< (const shared_ptr<MeasurementTask>& lhs, const shared_ptr<MeasurementTask>& rhs);
#endif /* MeasurementTask_hpp */
