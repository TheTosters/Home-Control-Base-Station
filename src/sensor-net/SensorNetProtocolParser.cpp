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

SensorNetProtocolParser::SensorNetProtocolParser(CommunicationLink* _link)
: link(_link), inParser(new InParser()),
  responseCmdToSensorType({
    {REMOTE_CMD_VALUE_TEMP_HISTORY, svtTemperature},
    {"VHM", svtHumidity}, //TODO: add to doc or remove
    {"VPM", svtPowerConsumption} //TODO: add to doc or remove
  }),
  logger(spdlog::get(COMMUNICATION_LOGGER_NAME)) {

}

Number SensorNetProtocolParser::sendSimpleCommand(shared_ptr<string> command, NumbersList arguments, bool* isError) {
  RemoteCommandBuilder builder(*command);
  for(auto arg : *arguments) {
    builder.addArgument(arg);
  }
  shared_ptr<string> response = link->sendCommand(builder.buildCommand());
  if (*response == "") {
    if (isError != nullptr) {
      *isError = true;
    }
    return Number((int)0);
  }
  shared_ptr<RemoteCommand> retCmd = inParser->parse(response);
  if (isError != nullptr) {
    *isError = retCmd == nullptr || (*retCmd == REMOTE_CMD_ERROR);
  }

  if ((retCmd != nullptr) && (retCmd->getArgumentsCount() < 0) &&
      ((retCmd->getArgType() == RemoteCommandArgumentType_DIGIT ||
        retCmd->getArgType() == RemoteCommandArgumentType_DIGIT_SEQUENCE)) ) {
    return retCmd->argument();
  }
  return Number((int)0);
}

void SensorNetProtocolParser::requestMeasurement(MeasurementMap& result, int count) {

  count = count <= 1 ? 1 : count;
  time_t now = time(nullptr);
  SensorValueType sensorType;

  for(PhysicalSensorType mesType : link->getDevice()->getType() ) {

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

shared_ptr<RemoteCommand> SensorNetProtocolParser::executeSimpleCommand(const string& cmd, bool* emptyResponse) {
  RemoteCommandBuilder builder(cmd);
  shared_ptr<string> response = link->sendCommand(builder.buildCommand());
  if (emptyResponse != nullptr) {
    *emptyResponse = (response == nullptr) || ((*response).length() == 0);
  }
  return inParser->parse(response);
}

void SensorNetProtocolParser::handleSensorCapabilities( shared_ptr<PhysicalSensor> sensor,
    shared_ptr<RemoteCommand> command) {

  uint64_t capFlags = command->argumentAsUInt(0);
  for(auto &cap : KNOWN_PHYSICAL_SENSOR_TYPES) {
    if ( (capFlags & cap) == cap) {
      sensor->addType(cap);
      capFlags &= ~cap;
    }
  }
  if (capFlags != 0) {
    logger->warn("Unknown sensor capabilities flags {}", capFlags);
  }
}

bool SensorNetProtocolParser::verifyCommand(shared_ptr<RemoteCommand> command, const string& expected,
    RemoteCommandArgumentType expectedType) {

  if (expectedType == RemoteCommandArgumentType_DIGIT_SEQUENCE) {
    bool isOk = (command.get() != nullptr) && (*command == expected) && (command->getArgumentsCount() > 0);
    if (isOk == true) {
      isOk &= (command->getArgType() == expectedType) || (command->getArgType() == RemoteCommandArgumentType_DIGIT);
    }
    return isOk;
  }

  return (command.get() != nullptr) && (*command == expected) && (command->getArgType() == expectedType) &&
      (command->getArgumentsCount() > 0);
}

bool SensorNetProtocolParser::requestSensorSpec(int& startingStep, bool& shouldRetry) {
  shared_ptr<PhysicalSensor> sensor = link->getDevice();
  //parameters: REMOTE_CMD_GET_SYSTEM_CAPABILITIES and REMOTE_CMD_GET_SOFTWARE_VERSION are mandatory!

  shared_ptr<RemoteCommand> command;

  //got empty line no error no nothing, probably communication problem
  shouldRetry = false;
  bool tmp = false;

  if (startingStep < 1) {
    command = executeSimpleCommand(REMOTE_CMD_GET_SOFTWARE_VERSION, &tmp);
    shouldRetry |= tmp;
    if (verifyCommand(command, REMOTE_CMD_GET_SOFTWARE_VERSION, RemoteCommandArgumentType_STRING)) {
      sensor->getMetadata()->softwareVersion = *(command->stringArgument());
      startingStep = 1;

    } else {
      return false;
    }
  }

  if (startingStep < 2) {
    command = executeSimpleCommand(REMOTE_CMD_GET_SYSTEM_CAPABILITIES, &tmp);
    shouldRetry |= tmp;
    if (verifyCommand(command, REMOTE_CMD_GET_SYSTEM_CAPABILITIES, RemoteCommandArgumentType_DIGIT_SEQUENCE)) {
      handleSensorCapabilities(sensor, command);
      startingStep = 2;
    } else {
      return false;
    }
  }

  if (startingStep < 3) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_TEMPERATURE_RESOLUTION, &tmp);
    shouldRetry |= tmp;
    startingStep =  tmp ? startingStep : 4;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_TEMPERATURE_RESOLUTION, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->temperatureResolution = command->argumentAsInt();
    }
  }

  if (startingStep < 4) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_TEMPERATURE_PERIOD, &tmp);
    shouldRetry |= tmp;
    startingStep =  tmp ? startingStep : 5;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_TEMPERATURE_PERIOD, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->temperaturePeriod = command->argumentAsInt();
    }
  }

  if (startingStep < 5) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_POWER_PERIOD, &tmp);
    shouldRetry |= tmp;
    startingStep = tmp ? startingStep : 6;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_POWER_PERIOD, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->powerPeroid = command->argumentAsInt();
    }
  }

  if (startingStep < 6) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_SYSTEM_TIME, &tmp);
    shouldRetry |= tmp;
    startingStep = tmp ? startingStep : 7;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_SYSTEM_TIME, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->nodeSystemTime = command->argumentAsUInt();
    }
  }

  if (startingStep < 7) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_NODE_NAME, &tmp);
    shouldRetry |= tmp;
    startingStep = tmp ? startingStep : 8;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_NODE_NAME, RemoteCommandArgumentType_STRING)) {
      sensor->setName(*(command->stringArgument()));
    }
  }

  if (startingStep < 8) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_SAVING_MODE, &tmp);
    shouldRetry |= tmp;
    startingStep = tmp ? startingStep : 9;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_SAVING_MODE, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->powerMode = static_cast<PhysicalSensorPowerSaveMode>(command->argumentAsInt());
    }
  }

  if (startingStep < 9) {
    command = executeSimpleCommand(REMOTE_CMD_CONFIGURE_SAVING_ACTIVITY, &tmp);
    shouldRetry |= tmp;
    startingStep = tmp ? startingStep : 10;
    if (verifyCommand(command, REMOTE_CMD_CONFIGURE_SAVING_ACTIVITY, RemoteCommandArgumentType_DIGIT)) {
      sensor->getMetadata()->powerActivity = static_cast<PhysicalSensorPowerSaveActivity>(command->argumentAsInt());
    }
  }

  return true;
}
