//
//  Schedule.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Schedule.hpp"
#include <fstream>
#include <algorithm>
#include <time.h> 
#include "StringHelper.hpp"
#include "JSONHelper.hpp"
#include "LogHelper.hpp"

const double UNKNOWN_TEMPERATURE = 18;

struct TemperatureIdentifierNameComparator {
  string name;
  
  TemperatureIdentifierNameComparator(string _name) : name(_name) {
  }
  
  bool operator ()(shared_ptr<TemperatureIdentifier> item) {
    return name == item->getName();
  }
};

bool scheduleItemCompare(shared_ptr<ScheduleItem> const &item1, shared_ptr<ScheduleItem> const &item2) {
  return item1->getAbsoluteTime() < item2->getAbsoluteTime();
}

Schedule::Schedule(json const& definition, TemperatureIdentifierList tempIdentifiers)
: tempId(tempIdentifiers) {
  for(auto iter = definition.begin(); iter != definition.end(); iter++) {
    parseSingleRule( *iter );
  }
  
  //sort schedule items in order of time
  sort(items.begin(), items.end(), scheduleItemCompare);
}

void Schedule::parseSingleRule(json const& json) {
  if (checkIfKeysExists(json, {"from", "temperature"}) == false){
    spdlog::get(MISC_LOGGER_NAME)->error("  Config, wrong format of line:{}", json.dump());
    return;
  }
    
  string startTimeStr = json["from"];
  string tempIdNameStr = json["temperature"];
  
  string prefix("every");
  if (equal(prefix.begin(), prefix.end(), startTimeStr.begin())) {
    if (pareseEveryDayInWeek( trim(startTimeStr.substr(prefix.size())), tempIdNameStr) == false) {
      spdlog::get(MISC_LOGGER_NAME)->error("  Config, wrong format of line:{}", json.dump());
    }
    return;
  }
  
  tm startTime;
  memset(&startTime, 0, sizeof(tm));
  if (strptime(startTimeStr.c_str(), "%a %H:%M", &startTime) == NULL) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Config, wrong format of line:{}", json.dump());
    return;
  }
  
  auto tempId = findTemperatureIdentifier(tempIdNameStr);
  if (tempId == NULL) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Config, undefined temperature identifier:{} (line:{})",
                                         tempIdNameStr, json.dump());
    return;
  }
  
  items.push_back( shared_ptr<ScheduleItem>(new ScheduleItem(tempId, startTime)) );
}

void Schedule::setTemperatureIds(TemperatureIdentifierList& tempId) {
  this->tempId = tempId;
}

TemperatureIdentifierList& Schedule::getTemperatureIds() {
  return tempId;
}

bool Schedule::pareseEveryDayInWeek(string timeStr, string tempIdNameStr) {
  tm startTime;
  memset(&startTime, 0, sizeof(tm));
  if (strptime(timeStr.c_str(), "%H:%M", &startTime) == NULL) {
    return false;
  }
  
  auto tempId = findTemperatureIdentifier(tempIdNameStr);
  if (tempId == NULL) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Config, undefined temperature identifier:{}", tempIdNameStr);
    return false;
  }
  
  for(int t = 0; t < 7; t++) {
    startTime.tm_wday = t;
    items.push_back( shared_ptr<ScheduleItem>( new ScheduleItem(tempId, startTime) ) );
  }
  return true;
}

shared_ptr<TemperatureIdentifier> Schedule::findTemperatureIdentifier(string const& name) {
  auto result = std::find_if(tempId->begin(), tempId->end(), TemperatureIdentifierNameComparator(name));
  return result == tempId->end() ? shared_ptr<TemperatureIdentifier>(nullptr) : *result;
}

void Schedule::dumpSchedule() {
  char str[150];
  for(auto it = items.begin(); it != items.end(); it ++) {
    const tm tmp = it->get()->getStartTime();
    strftime(str, 150, "%A %H:%M", &tmp);
    fprintf(stdout, "%s : %f deg C\n", str, it->get()->getTemperatureIdentifier()->getTemperature());
  }
}

shared_ptr<ScheduleItem> Schedule::findCurrent(struct tm* now) {
  
  long nowInMinutes = now->tm_min + now->tm_hour * 60;
  shared_ptr<ScheduleItem> result = shared_ptr<ScheduleItem>(nullptr);
  
  for(auto iter = items.begin(); iter != items.end(); iter ++) {
    if (iter->get()->getStartTime().tm_wday <= now->tm_wday) {
      if (iter->get()->minutesFromMidnight() <= nowInMinutes) {
        result = *iter;
      }
      
    } else {
      break;
    }
  }

  return result;
}

double Schedule::getDesiredTemperature() {
  time_t seconds;
  struct tm nowTime;
  time(&seconds);
  localtime_r(&seconds, &nowTime);
  
  shared_ptr<ScheduleItem> item = findCurrent(&nowTime);
  if (item == NULL) {
    return UNKNOWN_TEMPERATURE;
  }
  
  return item->getTemperatureIdentifier()->getTemperature();
}
