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
#include "ScheduleItem.hpp"
#include "TemperatureIdentifier.hpp"

using namespace std;

class Schedule {
  public:
    Schedule(string path);
  
    double getDesiredTemperature();
    void dumpSchedule();
  private:
    vector<shared_ptr<ScheduleItem>> items;
    vector<shared_ptr<TemperatureIdentifier>> tempId;

    void parseConfigLine(string line);
    void parseConfigId(string data);
    void parseConfigWeek(string data);
    bool pareseEveryDayInWeek(string timeStr, string tempId);
    shared_ptr<TemperatureIdentifier> findTemperatureIdentifier(string const& name);
    shared_ptr<ScheduleItem> findCurrent(struct tm* now);
};
#endif /* Schedule_hpp */
