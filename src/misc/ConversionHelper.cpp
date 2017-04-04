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
      return PhysicalSensorType_TEMPERATURE;
      
    case svtHumidity:
      return PhysicalSensorType_HUMIDITY;
      
    case svtPowerConsumption:
      return PhysicalSensorType_POWER_CONSUMPTION;
      
    default:
      throw std::invalid_argument("Can't map SensorValueType into PhysicalSensorType.");
  }
}

SensorValueType toSensorValueType(PhysicalSensorType value) {
  switch (value) {
    case PhysicalSensorType_TEMPERATURE:
      return svtTemperature;
      
    case PhysicalSensorType_HUMIDITY:
      return svtHumidity;
      
    case PhysicalSensorType_POWER_CONSUMPTION:
      return svtPowerConsumption;
      
    default:
      return svtUndefined;
  }
}
