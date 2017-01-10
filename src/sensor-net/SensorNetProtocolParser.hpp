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
#include "SensorValue.hpp"

using namespace std;

class CommunicationLink;

class SensorNetProtocolParser {
  public:
    SensorNetProtocolParser(CommunicationLink* link);
  
    void requestMeasurement(MeasurementMap& result, int count = 1);
  private:
    CommunicationLink*   link;
    unordered_map<string, SensorValueType> responseCmdToSensorType;
  
    bool parseInt(string const& str, int& result);
    bool parseDouble(string const& str, double &result);
  
    void logParseError(string const& data, string const& msg, size_t const& column);
  
    MeasurementList parseValueWithTimestamp(string const& data, size_t& startIndex, SensorValueType type, time_t now);
    bool detectResponseCommand(string const& data, size_t& startIndex, string& cmd, SensorValueType& sensorType);
    void parseMeasurementsRespons(string const& data, MeasurementMap& result);
};

#endif /* SensorNetProtocolParser_hpp */
