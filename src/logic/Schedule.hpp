//
//  Schedule.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Schedule_hpp
#define Schedule_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "logic/ScheduleItem.hpp"
#include "logic/TemperatureIdentifier.hpp"
#include "json/json.hpp"

using namespace std;
using namespace nlohmann;

class Schedule {
  public:
    Schedule(json const& definition, TemperatureIdentifierList tempIdentifiers);
  
    double getDesiredTemperature();
    void dumpSchedule();
    void setTemperatureIds(TemperatureIdentifierList& tempId);
    TemperatureIdentifierList& getTemperatureIds();
  private:
    vector<shared_ptr<ScheduleItem>> items;
    TemperatureIdentifierList tempId;

    void parseSingleRule(json const& json);
  
    void parseConfigLine(string line);
    void parseConfigId(string data);
    void parseConfigWeek(string data);
    bool pareseEveryDayInWeek(string timeStr, string tempId);
    shared_ptr<TemperatureIdentifier> findTemperatureIdentifier(string const& name);
    shared_ptr<ScheduleItem> findCurrent(struct tm* now);
};
#endif /* Schedule_hpp */
