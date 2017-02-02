//
//  ConversionHelper.cpp
//  HomeControl
//
//  Created by Bartłomiej on 02/02/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "misc/ConversionHelper.hpp"
#include <stdexcept>

PhysicalSensorType toPhysicalSensorType(SensorValueType value) {
  switch (value) {
    case svtTemperature:
      return TEMPERATURE;
      
    case svtHumidity:
      return HUMIDITY;
      
    case svtPowerConsumption:
      return POWER_CONSUMPTION;
      
    default:
      throw std::invalid_argument("Can't map SensorValueType into PhysicalSensorType.");
  }
}

SensorValueType toSensorValueType(PhysicalSensorType value) {
  switch (value) {
    case TEMPERATURE:
      return svtTemperature;
      
    case HUMIDITY:
      return svtHumidity;
      
    case POWER_CONSUMPTION:
      return svtPowerConsumption;
      
    default:
      return svtUndefined;
  }
}
