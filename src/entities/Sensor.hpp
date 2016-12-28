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

class Sensor : public Entity {
  public:
    Sensor();
    Sensor(long id, string name, Point* pos = NULL);
    Sensor(string name, Point* pos = NULL);
  
    Point*      getPosition();
    string      getName();
    time_t      getTimestamp();
    void        setPosition(Point p);
    void        setName(string const& name);
    long        getRoomId();
    void        setRoomId(long roomId);
    void        setAddress(string const& address);
    string      getAddress();
    void        setType(string const& type);
    string      getType();
  
    SensorValue getLastValue();
    void        addValue(SensorValue val);
  private:
    Point         position;
    string        name;
    string        address;    //it can be MAC, Bluetooth address, or other protocol dependent
    string        type;
    SensorValue   lastValue;
    long          roomId;
};

#endif /* Sensor_hpp */
