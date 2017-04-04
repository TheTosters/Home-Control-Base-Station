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
#include <unordered_map>
#include <vector>
#include "entities/Entity.hpp"

enum SensorValueType {
  svtUndefined,
  svtTemperature,
  svtHumidity,
  svtPowerConsumption
};

using namespace std;

typedef tuple<SensorValueType, double, time_t> Measurement;
typedef shared_ptr<vector<shared_ptr<Measurement>>> MeasurementList;
typedef shared_ptr<unordered_map<string, MeasurementList>> MeasurementMap;

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

typedef shared_ptr<vector<shared_ptr<SensorValue>>> SensorValueList;

#endif /* SensorValue_hpp */
