//
//  LogHelper.hpp
//  HomeControl
//
//  Created by Bartłomiej on 19/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef LogHelper_hpp
#define LogHelper_hpp

#include <stdio.h>
#include <string>
#include <spdlog/spdlog.h>

using namespace std;

static const string LOGIC_LOGGER_NAME = "Logic";
static const string COMMUNICATION_LOGGER_NAME = "Com";
static const string MISC_LOGGER_NAME = "Misc";
static const string WEB_LOGGER_NAME = "Web";
static const vector<string> loggerNames = {LOGIC_LOGGER_NAME, COMMUNICATION_LOGGER_NAME, MISC_LOGGER_NAME, WEB_LOGGER_NAME};

void createLoggers(int const logLevel, bool const &logToConsole, string const &logFile, string const& format);
void overrideLogger(string const& name, int* logLevel, string* format);
void overrideLoggersLevel(int const logLevel);

#endif /* LogHelper_hpp */
