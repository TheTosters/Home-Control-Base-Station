//
//  LogHelper.cpp
//  HomeControl
//
//  Created by Bartłomiej on 19/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "misc/LogHelper.hpp"
#include <vector>
#include <iostream>

using namespace spdlog;

void createLoggers(int const logLevel, bool const &logToConsole, string const &logFile, string const& format) {
  try {
    vector<sink_ptr> sinks;
    
    if (logToConsole) {
      sinks.push_back(make_shared<sinks::stdout_sink_st>());
    }
    
    if (logFile.length() > 0) {
      sinks.push_back(make_shared<sinks::daily_file_sink_st>(logFile, "log", 23, 59));
    }

    for(auto iter = loggerNames.begin(); iter != loggerNames.end(); iter++) {
      auto tmp = make_shared<logger>(*iter, begin(sinks), end(sinks));
      tmp->set_level( static_cast<level::level_enum>(logLevel) );
      if (format.length() > 0) {
        tmp->set_pattern(format);
      }
      register_logger(tmp);
    }
    
  } catch (const spdlog_ex& ex) {
    std::cout << "error: Log initialization failed: " << ex.what() << std::endl;
  }
}

void overrideLogger(string const& name, int* logLevel, string* format) {
  auto logger = spdlog::get(name);
  if (logger != nullptr) {
    if (logLevel != nullptr) {
      logger->set_level( static_cast<level::level_enum>(*logLevel) );
    }
    
    if (format != nullptr) {
      logger->set_pattern(*format);
    }
  }
}

void overrideLoggersLevel(int const logLevel) {
  set_level( static_cast<level::level_enum>(logLevel));
}
