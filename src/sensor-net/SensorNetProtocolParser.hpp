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
#include "sensor-net/parsers/InParser.hpp"
#include "entities/PhysicalSensor.hpp"

using namespace std;

class CommunicationLink;

class SensorNetProtocolParser {
  public:
    SensorNetProtocolParser(CommunicationLink* link);
  
    void requestMeasurement(MeasurementMap& result, int count = 1);
    bool sendPreamble();
    bool requestSensorSpec();
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
    shared_ptr<RemoteCommand> executeSimpleCommand(const string& cmd);
    void handleSensorCapabilities( shared_ptr<PhysicalSensor> sensor, shared_ptr<RemoteCommand> command);
    bool verifyCommand(shared_ptr<RemoteCommand> command, const string& expected, RemoteCommandArgumentType expectedType);
};

#endif /* SensorNetProtocolParser_hpp */
