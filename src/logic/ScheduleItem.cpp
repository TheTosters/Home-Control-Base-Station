//
//  ScheduleItem.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "ScheduleItem.hpp"

ScheduleItem::ScheduleItem(shared_ptr<TemperatureIdentifier> tempId, tm time)
: temperatureIdentifier(tempId), startTime(time) {
  
}

shared_ptr<TemperatureIdentifier> ScheduleItem::getTemperatureIdentifier() {
  return temperatureIdentifier;
}

tm ScheduleItem::getStartTime() const {
  return startTime;
}

long ScheduleItem::getAbsoluteTime() const {
  return (startTime.tm_wday * 60 * 24) + minutesFromMidnight();
}

long ScheduleItem::minutesFromMidnight() const {
  return startTime.tm_min + startTime.tm_hour * 60;
}