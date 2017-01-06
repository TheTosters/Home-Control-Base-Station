//
//  SensorNetProtocolParser.hpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef SensorNetProtocolParser_hpp
#define SensorNetProtocolParser_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "SensorValue.hpp"

using namespace std;

class CommunicationLink;

typedef tuple<SensorValueType, double, time_t> Measurement;
typedef shared_ptr<vector<shared_ptr<Measurement>>> MeasurementList;

class SensorNetProtocolParser {
  public:
    SensorNetProtocolParser(CommunicationLink* link);
  
    void requestMeasurement(unordered_map<string, MeasurementList>& result, int count = 1);
  private:
    CommunicationLink*   link;
    unordered_map<string, SensorValueType> responseCmdToSensorType;
  
    bool parseValueWithTimestamp(shared_ptr<string> data, string::iterator& it, double& value, time_t& timeStamp);
    SensorValueType parseSensorValueType(shared_ptr<string> data, string::iterator& iter);
    bool parseInt(string const& str, int& result);
    bool parseDouble(string const& str, double &result);
  
    void logParseError(string const& data, string const& msg, size_t const& column);
  
    MeasurementList parseValueWithTimestamp(string const& data, size_t& startIndex, SensorValueType type);
    bool detectResponseCommand(string const& data, size_t& startIndex, string& cmd, SensorValueType& sensorType);
    void parseMeasurementsRespons(string const& data, unordered_map<string, MeasurementList>& result);

};

#endif /* SensorNetProtocolParser_hpp */
