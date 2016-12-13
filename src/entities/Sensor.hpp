//
//  Sensor.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Sensor_hpp
#define Sensor_hpp

#include <stdio.h>
#include <string>
#include "SensorValue.hpp"
#include "Point.hpp"

using namespace std;

class Sensor : Entity {
  public:
    Sensor(int id, string name, Point* pos = NULL);
    Sensor(string name, Point* pos = NULL);
  
    Point       getPosition();
    string      getName();
    time_t      getTimestamp();
    void        setPosition(Point p);
    void        setName(string name);
  
    SensorValue getLastValue();
    void        addValue(SensorValue val);
  private:
    Point         position;
    string        name;
    SensorValue   lastValue;
};

#endif /* Sensor_hpp */
