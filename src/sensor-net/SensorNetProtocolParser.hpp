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
#include "entities/SensorValue.hpp"
#include "misc/LogHelper.hpp"

using namespace std;

class InParser;
class CommunicationLink;
class RemoteCommand;

class SensorNetProtocolParser {
  public:
    SensorNetProtocolParser(CommunicationLink* link);
  
    void requestMeasurement(MeasurementMap& result, int count = 1);
    void sendPreamble();
  private:
    CommunicationLink* link;
    InParser* inParser;
    unordered_map<string, SensorValueType> responseCmdToSensorType;
    shared_ptr<spdlog::logger> logger;
  
    bool parseInt(string const& str, int& result);
    bool parseDouble(string const& str, double &result);
  
    void logParseError(string const& data, string const& msg, size_t const& column);
  
    MeasurementList parseValueWithTimestamp(RemoteCommand& command, SensorValueType type, time_t now);
    void detectResponseCommand(RemoteCommand& command, SensorValueType& sensorType);
    void parseMeasurementsRespons(string const& data, MeasurementMap& result);
};

#endif /* SensorNetProtocolParser_hpp */
