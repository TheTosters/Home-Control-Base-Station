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
    Sensor(long id = -1);
  
    string      getName();
    void        setName(string const& name);
    long        getRoomId();
    void        setRoomId(long roomId);
    void        setPhysicalSensorId(long id);
    long        getPhysicalSensorId();
    void        setX(int x);
    int         getX();
    void        setY(int y);
    int         getY();
  private:
    int           posX;
    int           posY;
    string        name;
    long          physicalSensorId;
    long          roomId;
};

#endif /* Sensor_hpp */
