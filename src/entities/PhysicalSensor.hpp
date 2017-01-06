//
//  PhysicalSensor.hpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef PhysicalSensor_hpp
#define PhysicalSensor_hpp

#include <stdio.h>
#include <string>
#include "Entity.hpp"

using namespace std;

enum PhysicalSensorType {
  TEMPERATURE,
  HUMIDITY,
  POWER_CONSUMPTION
};

/**
 * This class is for having instance of other physical device which we can contact to get measurements.
 * It's not this same as Sensor, which is more like logical representation of Physical sensor in rooms.
 * This object should have all properties needed to communicate with physical device, such as encryption keys etc.
 * NOTE: Id is retrieved from physical device (same as address) it is possible to have collisions, this is
 * communication/logic layer resposibility to not allow such situation. Since this object id is used in SensorValue
 * this can bring total mess in db. Be warned.
 */
class PhysicalSensor : public Entity {
  public:
    PhysicalSensor();
  
    void    setName(string const& name);
    string& getName();
  
    void    setAddress(string const& address);
    string& getAddress();
  
    void    setType(PhysicalSensorType type);
    PhysicalSensorType getType();
  
    void    setLastFetchTime(time_t value);
    time_t  getLastFetchTime();
  
    void    setDesiredFetchDelay(time_t value);
    time_t  getDesiredFetchDelay();
  
    void    setLastMeasurementTime(time_t value);
    time_t  getLastMeasurementTime();
  
    void    setLastValue(double value);
    double  getLastValue();
  
    void    setPreviousValue(double value);
    double  getPreviousValue();
  protected:
    string              name;
    string              address;    //it can be MAC, Bluetooth address, or other protocol dependent
    PhysicalSensorType  type;
  
    /** Timestamp of last connection to physical device with request for measurements */
    time_t              lastFetchTime;
  
    /** Config related value, informs about intervals in which communication with Physical device should 
     be done. It's minimal value which should be waited before new fetch is tried */
    time_t              desiredFetchDelay;
  
    /** Time reported by physical device, when measurement was done. Do not confuse with lastFetchTime.
     Each device marks time when measurement is done, it has nothing to do with time when this info is
     feteched from physical device */
    time_t              lastMeasurementTime;
  
    /** Value read from physical device at lastFetchTime. This is the newest obtained value from physical device */
    double              lastValue;
  
    /** Previous value, it's copy of lastValue before fetching new data is done */
    double              previousValue;
};
#endif /* PhysicalSensor_hpp */
