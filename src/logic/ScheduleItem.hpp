//
//  ScheduleItem.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef ScheduleItem_hpp
#define ScheduleItem_hpp

#include <stdio.h>
#include <time.h>
#include "TemperatureIdentifier.hpp"

using namespace std;

class ScheduleItem {
  public:
    ScheduleItem(shared_ptr<TemperatureIdentifier> tempId, tm time);
  
    shared_ptr<TemperatureIdentifier> getTemperatureIdentifier();
    tm getStartTime() const;
    long getAbsoluteTime() const;
    long minutesFromMidnight() const;
  private:
    shared_ptr<TemperatureIdentifier> temperatureIdentifier;
    tm                                startTime;
};

#endif /* ScheduleItem_hpp */
