//
//  SensorValue.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SensorValue_hpp
#define SensorValue_hpp

#include <stdio.h>
#include <time.h>
#include "Entity.hpp"

enum SensorValueType {
  svtUndefined,
  svtTemperature,
  svtHumidity,
  svtPowerConsumption
};

class SensorValue : public Entity {
  public:
    SensorValue();
    SensorValue(long id, long sensId, double value, SensorValueType valType, time_t timestamp);
  
    double getValue();
    time_t getTimestamp();

    void setValue(double val);
    void setTimestamp(time_t time);
  
    long getPhysicalSensorId();
    void setPhysicalSensorId(long sensorId);
  
    SensorValueType getValueType();
    void setValueType(SensorValueType value);
  private:
    long    physicalSensorId;   //Note: this is PhysicalSensor id, not SensorId
    double  value;
    time_t  timestamp;
    SensorValueType valueType;
};

#endif /* SensorValue_hpp */
