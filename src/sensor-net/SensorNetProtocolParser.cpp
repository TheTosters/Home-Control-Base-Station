//
//  SensorNetProtocolParser.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "sensor-net/SensorNetProtocolParser.hpp"
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "sensor-net/CommunicationLink.hpp"
#include "misc/LogHelper.hpp"
#include "parsers/RemoteCommandBuilder.hpp"
#include "parsers/InParser.hpp"

const string SINGLE_MEASUREMENT_REQUEST = "RDR0001";
const string MEASUREMENT_COMMAND = "RDR";

SensorNetProtocolParser::SensorNetProtocolParser(CommunicationLink* _link)
: link(_link), inParser(new InParser()),
  responseCmdToSensorType({
    {"VTM", svtTemperature},
    {"VHM", svtHumidity},
    {"VPM", svtPowerConsumption}
  }),
  logger(spdlog::get(COMMUNICATION_LOGGER_NAME)) {

}

void SensorNetProtocolParser::sendPreamble() {
  link->sendCommand("!!!!#");
}

void SensorNetProtocolParser::requestMeasurement(MeasurementMap& result, int count) {

  count = count <= 1 ? 1 : count;
  time_t now = time(nullptr);
  SensorValueType sensorType;

  for(int mesType = PhysicalSensorType_BEGIN; mesType < PhysicalSensorType_END; mesType ++) {

    if (link->getDevice()->isType((PhysicalSensorType)mesType) == false) {
      continue;
    }

    RemoteCommandBuilder* builder;
    switch(mesType) {
      case PhysicalSensorType_TEMPERATURE:
        builder = new RemoteCommandBuilder(REMOTE_CMD_READ_TEMP_HISTORY);
        break;

      case PhysicalSensorType_HUMIDITY:
      case PhysicalSensorType_POWER_CONSUMPTION:
        logger->error("Not implemented!");
        break;

      default:
        continue;
    }

    builder->addArgument(count);
    shared_ptr<string> response = link->sendCommand(builder->buildCommand());
    delete builder;

    shared_ptr<RemoteCommand> command = inParser->parse(response);

    detectResponseCommand(*command, sensorType);
    if (sensorType != svtUndefined) {
      MeasurementList measurements = parseValueWithTimestamp(*command, sensorType, now);
      if (measurements != nullptr) {
        (*result)[*(command->getCommand())] = measurements;
      }
    }
  }
}

void SensorNetProtocolParser::detectResponseCommand(RemoteCommand& command, SensorValueType& sensorType) {
  sensorType = svtUndefined;
  shared_ptr<string> cmd = command.getCommand();
  if (responseCmdToSensorType.find(*cmd) == responseCmdToSensorType.end()) {
    sensorType = svtUndefined;

  } else {
    sensorType = responseCmdToSensorType[*cmd];
  }
}

MeasurementList SensorNetProtocolParser::parseValueWithTimestamp(RemoteCommand& command, SensorValueType type, time_t now) {
  MeasurementList result = make_shared<vector<shared_ptr<Measurement>>>();
  if (command.getArgType() != RemoteCommandArgumentType_DIGIT_MULTI_SEQUENCE) {
    logger->warn("Expected RemoteCommandArgumentType_DIGIT_MULTI_SEQUENCE type");
    return nullptr;
  }

  for(unsigned int t = 0; t < command.getArgumentsCount(); t++) {
    shared_ptr<vector<Number> > sequence = command.getDigitSequence(t);
    if (sequence->size() != 2) {
      logger->warn("Expected 2 elements in sequence");
      continue;
    }

    int tmpInt = (*sequence)[0].asUInt64();
    time_t timeStamp = now - static_cast<time_t>(tmpInt);
    double value = (*sequence)[1].asDouble();

    result->push_back(make_shared<Measurement>(type, value, timeStamp));
  }
  
  return result;
}

void SensorNetProtocolParser::logParseError(string const& data, string const& msg, size_t const& column) {
  
  logger->warn("Parse error near {}: {}", column, msg);
  logger->warn("Received data:{}", data);
  string tmp = string(column + 14, ' ');
  tmp.append("^");
  logger->warn("{}", tmp);
}
