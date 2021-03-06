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
    bool requestSensorSpec(int& startingStep, bool& shouldRetry);
    Number sendSimpleCommand(shared_ptr<string> command, NumbersList arguments, bool* isError);
  private:
    CommunicationLink* link;
    InParser* inParser;
    unordered_map<string, SensorValueType> responseCmdToSensorType;
    shared_ptr<spdlog::logger> logger;

    MeasurementList parseValueWithTimestamp(RemoteCommand& command, SensorValueType type, time_t now);
    void detectResponseCommand(RemoteCommand& command, SensorValueType& sensorType);
    void parseMeasurementsRespons(string const& data, MeasurementMap& result);
    shared_ptr<RemoteCommand> executeSimpleCommand(const string& cmd, bool* emptyResponse);
    void handleSensorCapabilities( shared_ptr<PhysicalSensor> sensor, shared_ptr<RemoteCommand> command);
    bool verifyCommand(shared_ptr<RemoteCommand> command, const string& expected,
        RemoteCommandArgumentType expectedType);
};

#endif /* SensorNetProtocolParser_hpp */
