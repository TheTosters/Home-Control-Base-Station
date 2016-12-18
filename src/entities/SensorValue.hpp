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

class SensorValue : public Entity {
  public:
    SensorValue(int id, double value, time_t timestamp);
    SensorValue(double value, time_t timestamp);
  
    double getValue();
    time_t getTimestamp();

    void setValue(double val);
    void setTimestamp(time_t time);
  private:
    double  value;
    time_t  timestamp;
};

#endif /* SensorValue_hpp */
