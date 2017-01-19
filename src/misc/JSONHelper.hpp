//
//  JSONHelper.hpp
//  HomeControl
//
//  Created by Bartłomiej on 28/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef JSONHelper_hpp
#define JSONHelper_hpp

#include <stdio.h>
#include "json.hpp"
#include "Entities.hpp"

using namespace std;
using namespace nlohmann;

json toJSON(shared_ptr<Point> point);
json toJSON(shared_ptr<Room> room);
json toJSON(shared_ptr<Sensor> sensor);
json toJSON(shared_ptr<SensorValue> sensorValue, bool includeSensorId);
json toJSON(RoomsList roomsList);
json toJSON(PhysicalSensorList const& list);
json toJSON(SensorValueList const& list, bool includeSensorId);

shared_ptr<Point> pointFromJSON(string const& data);
shared_ptr<Sensor> sensorFromJSON(string const& data);
shared_ptr<Room> roomFromJSON(string const& data);
shared_ptr<vector<shared_ptr<Room>>> roomListFromJSON(string const& data);
shared_ptr<PhysicalSensor> physicalSensorFromJSON(json const& data);
PhysicalSensorList physicalSensorsFromJSON(string const& data);

long getOptionalJSONLong(json json, string const& key);
shared_ptr<string> getOptionalJSONString(json json, string const& key);
void getOptionalJSONBool(json json, string const& key, bool& value);

bool checkIfKeysExists(json json, vector<string> const& keys, string* missing = nullptr);
#endif /* JSONHelper_hpp */
