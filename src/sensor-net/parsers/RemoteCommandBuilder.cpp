/*
 * RemoteCommandBuilder.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Zarnowski
 */

#include "RemoteCommandBuilder.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>

//config
const string REMOTE_CMD_CONFIGURE_TEMPERATURE_RESOLUTION = "CTR";
const string REMOTE_CMD_CONFIGURE_TEMPERATURE_PERIOD = "CTP";
const string REMOTE_CMD_CONFIGURE_POWER_PERIOD = "CPP";
const string REMOTE_CMD_CONFIGURE_SYSTEM_TIME = "CST";
const string REMOTE_CMD_GET_SYSTEM_CAPABILITIES = "CSC";
const string REMOTE_CMD_CONFIGURE_NODE_NAME = "CNN";
const string REMOTE_CMD_CONFIGURE_SAVING_MODE = "CSM";
const string REMOTE_CMD_CONFIGURE_SAVING_ACTIVITY = "CSA";
const string REMOTE_CMD_CONFIGURE_DEFAULT_GPIO_OUTPUT = "COD";

//aux
const string REMOTE_CMD_REQUEST_SYSTEM_RESET = "ARS";
const string REMOTE_CMD_GET_SOFTWARE_VERSION = "AVI";
const string REMOTE_CMD_CHANGE_OUTPUT_GPIO = "AOS";

//read
const string REMOTE_CMD_READ_TEMP_HISTORY = "RTH";

//values
const string REMOTE_CMD_VALUE_TEMP_HISTORY = "VTM";

//errors
const string REMOTE_CMD_ERROR = "ERR";

RemoteCommandBuilder::RemoteCommandBuilder(const string& cmd, bool includePreamble)
: outCmd(cmd), elementsType(UNKNOWN), isSequenceOpen(false), needComa(false), expectedNextSubsequence(false),
  includePreamble(includePreamble) {

    for (auto c = outCmd.begin() ; c < outCmd.end(); c++) {
        if (*c < 'A' || *c > 'Z') {
            throw invalid_argument("Only capital letters allowed!");
        }
    }
    if (outCmd.size() != 3) {
        throw invalid_argument("Cmd must be exactly 3 chars long!");
    }
}
void RemoteCommandBuilder::addArgument(int value) {
    addArgument((int64_t)value);
}
void RemoteCommandBuilder::addArgument(int64_t value) {
    if (elementsType == STRING) {
        throw invalid_argument("Digit expected, no mixed sequences are allowed!");
    }
    if (expectedNextSubsequence == true) {
        throw invalid_argument("Not allowed, call startSequence() first!");
    }
    elementsType = DIGIT;
    if (needComa == true) {
        outCmd += ',';
    }
    outCmd += to_string(value);
    needComa = true;
}

void RemoteCommandBuilder::addArgument(Number value) {
  if (value.getType() == DoubleType) {
    addArgument(value.asDouble());

  } else {
    addArgument(value.asInt64());
  }
}

void RemoteCommandBuilder::addArgument(double value) {
    if (elementsType == STRING) {
        throw invalid_argument("Digit expected, no mixed sequences are allowed!");
    }
    if (expectedNextSubsequence == true) {
        throw invalid_argument("Not allowed, call startSequence() first!");
    }
    elementsType = DIGIT;
    if (needComa == true) {
        outCmd += ',';
    }
    stringstream str;
    str << fixed << setprecision( 3 ) << value;
    outCmd += str.str();
    needComa = true;
}

void RemoteCommandBuilder::addArgument(const string& value) {
    if (elementsType == DIGIT) {
        throw invalid_argument("String expected, no mixed sequences are allowed!");
    }
    if (expectedNextSubsequence == true) {
        throw invalid_argument("Not allowed, call startSequence() first!");
    }
    elementsType = STRING;
    if (needComa == true) {
        outCmd += ',';
    }
    outCmd += '"';
    outCmd += value;
    outCmd += '"';
    needComa = true;
}

void RemoteCommandBuilder::startSequence() {
    if (isSequenceOpen == true) {
        throw invalid_argument("No nested sequences allowed!");
    }
    isSequenceOpen = true;
    outCmd += "(";
    needComa = false;
    expectedNextSubsequence = false;
}

void RemoteCommandBuilder::endSequence() {
    if (isSequenceOpen == false) {
        throw invalid_argument("No sequence is open!");
    }
    if (needComa == false) {
        throw invalid_argument("At least one element in sequence is required!");
    }
    isSequenceOpen = false;
    outCmd += ")";
    needComa = false;
    expectedNextSubsequence = true;
}

string RemoteCommandBuilder::buildCommand() {
    if (isSequenceOpen == true) {
        throw invalid_argument("Last sequence is still open, call endSequence()!");
    }
    string tmp = includePreamble ? "!!!#" : "";
    tmp += outCmd;
    tmp += "\r";
    return tmp;
}
