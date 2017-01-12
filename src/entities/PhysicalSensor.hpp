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
#include <vector>
#include "Entity.hpp"
#include "SensorValue.hpp"

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
  
    void    addType(PhysicalSensorType type);
    vector<PhysicalSensorType>& getType();
  
    void    setLastFetchTime(time_t value);
    time_t  getLastFetchTime();
  
    void    setDesiredFetchDelay(time_t value);
    time_t  getDesiredFetchDelay();
  
    void    setLastMeasurements(MeasurementMap data);
    MeasurementList& getLastMeasurements();
  protected:
    string              name;
    string              address;    //it can be MAC, Bluetooth address, or other protocol dependent
    vector<PhysicalSensorType>  types;
  
    /** Timestamp of last connection to physical device with request for measurements */
    time_t              lastFetchTime;
  
    /** Config related value, informs about intervals in which communication with Physical device should 
     be done. It's minimal value which should be waited before new fetch is tried */
    time_t              desiredFetchDelay;

    /** List with the newest values fetch from physical device */
    MeasurementList     lastMeasurements;
  
    void updateLastMeasurement(SensorValueType valType, double value, time_t time);
};

typedef vector<shared_ptr<PhysicalSensor>> PhysicalSensorVector;
typedef shared_ptr<PhysicalSensorVector> PhysicalSensorList;
#endif /* PhysicalSensor_hpp */
