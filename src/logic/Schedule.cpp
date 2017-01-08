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

const double UNKNOWN_TEMPERATURE = 18;

struct TemperatureIdentifierNameComparator {
  string name;
  
  TemperatureIdentifierNameComparator(string _name) : name(_name) {
  }
  
  bool operator ()(shared_ptr<TemperatureIdentifier> item) {
    return name == item->getName();
  }
};

string trim(const string &s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && isspace(*it))
    it++;
  
  std::string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && isspace(*rit))
    rit++;
  
  return std::string(it, rit.base());
}

bool scheduleItemCompare(shared_ptr<ScheduleItem> const &item1, shared_ptr<ScheduleItem> const &item2) {
  return item1->getAbsoluteTime() < item2->getAbsoluteTime();
}

Schedule::Schedule(string path) {
  ifstream infile(path);
  if (infile.good() == false) {
    //todo:logging
    fprintf(stderr, "Can't open config for schedule: %s\n", path.c_str());
    throw invalid_argument("No schedule config file");
  }
  
  string line;
  while (std::getline(infile, line)) {
    parseConfigLine(line);
  }
  infile.close();
  //sort schedule items in order of time
  sort(items.begin(), items.end(), scheduleItemCompare);
}

void Schedule::parseConfigLine(string line) {
  if (line.size() == 0) {
    //just skip empty lines
    return;
  }
  size_t index = line.find(":");
  if (index == string::npos) {
    fprintf(stderr, "Config, wrong format of line: %s\n", line.c_str());
    return;
  }
  
  string prefix = line.substr(0, index);
  transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);
  
  string data = line.substr(index + 1);
  
  transform(data.begin(), data.end(), data.begin(), ::tolower);
  
  if (prefix == "id") {
    parseConfigId(data);
    
  } else if (prefix == "week") {
    parseConfigWeek(data);
    
  } else {
    fprintf(stderr, "Config, illegal type od data: %s\n", line.c_str());

  }
}

void Schedule::parseConfigId(string data) {
  size_t index = data.find(",");
  if (index == string::npos) {
    fprintf(stderr, "Config, wrong format of line: %s\n", data.c_str());
    return;
  }
  string name = trim(data.substr(0, index));
  double temp = atof( trim(data.substr(index + 1)).c_str() );
  tempId.push_back( shared_ptr<TemperatureIdentifier>( new TemperatureIdentifier(name, temp) ) );
}

void Schedule::parseConfigWeek(string data) {
  size_t index = data.find(",");
  if (index == string::npos) {
    fprintf(stderr, "Config, wrong format of line: %s\n", data.c_str());
    return;
  }
  string startTimeStr = trim(data.substr(0, index));
  string tempIdNameStr = trim(data.substr(index + 1));
  
  string prefix("every");
  if (equal(prefix.begin(), prefix.end(), startTimeStr.begin())) {
    if (pareseEveryDayInWeek( trim(startTimeStr.substr(prefix.size())), tempIdNameStr) == false) {
      fprintf(stderr, "Config, wrong format of line: %s\n", data.c_str());
    }
    return;
  }
  
  tm startTime;
  memset(&startTime, 0, sizeof(tm));
  if (strptime(startTimeStr.c_str(), "%a %H:%M", &startTime) == NULL) {
    fprintf(stderr, "Config, wrong format of line: %s\n", data.c_str());
    return;
  }
  
  auto tempId = findTemperatureIdentifier(tempIdNameStr);
  if (tempId == NULL) {
    fprintf(stderr, "Config, undefined temperature identifier: %s (line:%s)\n", tempIdNameStr.c_str(), data.c_str());
    return;
  }
  
  items.push_back( shared_ptr<ScheduleItem>(new ScheduleItem(tempId, startTime)) );
}

bool Schedule::pareseEveryDayInWeek(string timeStr, string tempIdNameStr) {
  tm startTime;
  memset(&startTime, 0, sizeof(tm));
  if (strptime(timeStr.c_str(), "%H:%M", &startTime) == NULL) {
    return false;
  }
  
  auto tempId = findTemperatureIdentifier(tempIdNameStr);
  if (tempId == NULL) {
    fprintf(stderr, "Config, undefined temperature identifier: %s\n", tempIdNameStr.c_str());
    return false;
  }
  
  for(int t = 0; t < 7; t++) {
    startTime.tm_wday = t;
    items.push_back( shared_ptr<ScheduleItem>( new ScheduleItem(tempId, startTime) ) );
  }
  return true;
}

shared_ptr<TemperatureIdentifier> Schedule::findTemperatureIdentifier(string const& name) {
  auto result = std::find_if(tempId.begin(), tempId.end(), TemperatureIdentifierNameComparator(name));
  return result == tempId.end() ? shared_ptr<TemperatureIdentifier>(nullptr) : *result;
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
