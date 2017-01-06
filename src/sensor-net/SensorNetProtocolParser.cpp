//
//  SensorNetProtocolParser.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "SensorNetProtocolParser.hpp"
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include "CommunicationLink.hpp"

const string SINGLE_MEASUREMENT_REQUEST = "RDR0001";
const string MEASUREMENT_COMMAND = "RDR";

SensorNetProtocolParser::SensorNetProtocolParser(CommunicationLink* _link)
: link(_link),
  responseCmdToSensorType({
    {"VTH", svtTemperature},
    {"VHH", svtHumidity},
    {"VPH", svtPowerConsumption}
  })
{
    
}

bool SensorNetProtocolParser::parseInt(string const& str, int& result) {
  char* p;
  long tmp = strtol( str.c_str(), &p, 16 );
  if ( *p != 0) {
    return false;
  }
  if (tmp > 0x7fff) {
    tmp -= 0x10000;
  }
  result = static_cast<int>(tmp);
  return true;
}

bool SensorNetProtocolParser::parseDouble(string const& str, double &result) {
  char* p;
  long tmp = strtol( str.c_str(), &p, 16 );
  if ( *p != 0) {
    return false;
  }
  if (tmp > 0x7fff) {
    tmp -= 0x10000;
  }
  int integral = static_cast<int>(tmp >> 4);
  int frac = static_cast<int>(tmp & 0b1111);
  result = integral + frac / 16.0f;
  return true;
}

void SensorNetProtocolParser::requestMeasurement(unordered_map<string, MeasurementList>& result, int count) {
  
  shared_ptr<string> response;
  
  if (count <= 1) {
    response = link->sendCommand(SINGLE_MEASUREMENT_REQUEST);
    
  } else {
    stringstream stream;
    stream << MEASUREMENT_COMMAND << std::nouppercase << std::setfill('0') << std::setw(4) << std::hex << count;
    response = link->sendCommand(stream.str());
  }

  parseMeasurementsRespons(*response, result);
}

bool SensorNetProtocolParser::detectResponseCommand(string const& data, size_t& startIndex, string& cmd, SensorValueType& sensorType) {
  if (data.length() < 4 || data.length() <= startIndex + 3) {
    sensorType = svtUndefined;
    cmd.clear();
    startIndex = data.length();
    return false;
  }
  
  cmd = data.substr(startIndex, 3);
  startIndex += 3;
  
  if (responseCmdToSensorType.find(cmd) == responseCmdToSensorType.end()) {
    sensorType = svtUndefined;
    
  } else {
    sensorType = responseCmdToSensorType[cmd];
  }
  
  return true;
}

MeasurementList SensorNetProtocolParser::parseValueWithTimestamp(string const& data, size_t& startIndex, SensorValueType type) {
  MeasurementList result = make_shared<vector<shared_ptr<Measurement>>>();
  
  while (startIndex < data.length() && data[startIndex] == '(') {
    
    startIndex++;
    size_t comaPos = data.find(",", startIndex);
    if (comaPos == string::npos) {
      logParseError(data, "Expected ',' character after this column.", startIndex);
      break;
    }
    
    int tmpInt;
    bool tmp = parseInt( data.substr(startIndex, comaPos - startIndex), tmpInt );
    if (tmp == false) {
      logParseError(data, "Can't convert this string into int.", startIndex);
      break;
    }
    time_t timeStamp = static_cast<user_time_t>(tmpInt);
    
    size_t endPos = data.find(")", comaPos);
    if (endPos == string::npos) {
      logParseError(data, "Expected ')' character after this column.", startIndex);
      break;
    }
    
    comaPos++;
    double value;
    tmp = parseDouble( data.substr(comaPos, endPos - comaPos), value );
    if (tmp == false) {
      logParseError(data, "Can't convert this string into double.", startIndex);
      break;
    }
    
    startIndex = endPos + 1;
    result->push_back(make_shared<Measurement>(type, value, timeStamp));
  }
  
  return result;
}

void SensorNetProtocolParser::parseMeasurementsRespons(string const& data, unordered_map<string, MeasurementList>& result) {
  size_t startIndex = 0;
  string cmd;
  SensorValueType sensorType;
  
  while(detectResponseCommand(data, startIndex, cmd, sensorType)) {
    MeasurementList measurements = parseValueWithTimestamp(data, startIndex, sensorType);
    if (measurements != nullptr) {
      result[cmd] = measurements;
    }
  }
}

void SensorNetProtocolParser::logParseError(string const& data, string const& msg, size_t const& column) {
  cerr << "Parse error near " << column << ":" << msg << endl;
  cerr << "Received data:" << data << endl;
  string tmp = string(column + 14, ' ');
  tmp.append("^");
  cerr << tmp << endl;
}