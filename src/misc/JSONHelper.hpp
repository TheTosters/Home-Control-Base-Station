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
json toJSON(shared_ptr<SensorValue> sensor);
json toJSON(shared_ptr<vector<shared_ptr<Room>>> roomsList);

shared_ptr<Point> pointFromJSON(string const& data);
shared_ptr<Sensor> sensorFromJSON(string const& data);
shared_ptr<Room> roomFromJSON(string const& data);
shared_ptr<vector<shared_ptr<Room>>> roomListFromJSON(string const& data);

long getOptionalJSONLong(json json, string const& key);
shared_ptr<string> getOptionalJSONString(json json, string const& key);

bool checkIfKeysExists(json json, vector<string> const& keys);
#endif /* JSONHelper_hpp */
