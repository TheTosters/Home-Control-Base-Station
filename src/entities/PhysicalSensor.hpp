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
#include <inttypes.h>
#include "entities/Entity.hpp"
#include "entities/SensorValue.hpp"
#include <unordered_set>
#include <mutex>

using namespace std;

enum PhysicalSensorType {
  PhysicalSensorType_TEMPERATURE = 1U << 0,
  PhysicalSensorType_POWER_SOURCE_LEVEL = 1U << 8,
  PhysicalSensorType_HUMIDITY,
  PhysicalSensorType_POWER_CONSUMPTION,
};

enum PhysicalSensorPowerSaveMode {
  PhysicalSensorPowerSaveMode_1
};

enum PhysicalSensorPowerSaveActivity {
  PhysicalSensorPowerSaveActivity_1
};

extern const unordered_set<PhysicalSensorType, hash<int>> KNOWN_PHYSICAL_SENSOR_TYPES;

class PhysicalSensorMetaData {
  public:
    string softwareVersion;

    PhysicalSensorPowerSaveMode powerMode;
    PhysicalSensorPowerSaveActivity powerActivity;
    int powerPeroid;

    int temperatureResolution;
    int temperaturePeriod;

    uint64_t nodeSystemTime;

    PhysicalSensorMetaData();
};
/**
 * This class is for having instance of other physical device which we can contact to get measurements.
 * It's not this same as Sensor, which is more like logical representation of Physical sensor in rooms.
 * This object should have all properties needed to communicate with physical device, such as encryption keys etc.
 * NOTE: Id is retrieved from physical device (same as address) it is possible to have collisions, this is
 * communication/logic layer responsibility to not allow such situation. Since this object id is used in SensorValue
 * this can bring total mess in db. Be warned.
 * WARNING: This object is SEMI THREAD SAFE it has it's own recursive mutex which are used for fields set/get but it's
 * possible to get live collection which is not guarded, int this case outer logic is responsible for data safety
 * (consider usage of lock()/unlock() methods of this object)!
 */
class PhysicalSensor : public Entity {
  public:
    PhysicalSensor();
    virtual ~PhysicalSensor();
  
    void    setName(string const& name);
    string  getName();
  
    void    setAddress(string const& address);
    string  getAddress();
  
    void    addType(PhysicalSensorType type);
    vector<PhysicalSensorType> getType();
  
    void    setLastFetchTime(time_t value);
    time_t  getLastFetchTime();
  
    void    setDesiredFetchDelay(time_t value);
    time_t  getDesiredFetchDelay();
  
    void    setLastMeasurements(MeasurementMap data);

    //warning: returns live inner collection!
    MeasurementList& getLastMeasurements();
  
    bool    isType(PhysicalSensorType type);
    shared_ptr<Measurement> getLastMeasurement(PhysicalSensorType type);
    PhysicalSensorMetaData* getMetadata();

    void lock();
    void unlock();
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

    PhysicalSensorMetaData* metaData;
  
    recursive_mutex     sensorMutex;

    void updateLastMeasurement(SensorValueType valType, double value, time_t time);
};

typedef vector<shared_ptr<PhysicalSensor>> PhysicalSensorVector;
typedef shared_ptr<PhysicalSensorVector> PhysicalSensorList;
#endif /* PhysicalSensor_hpp */
