//
//  ConversionHelper.hpp
//  HomeControl
//
//  Created by Bartłomiej on 02/02/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef ConversionHelper_hpp
#define ConversionHelper_hpp

#include <stdio.h>
#include "entities/PhysicalSensor.hpp"
#include "entities/SensorValue.hpp"

PhysicalSensorType toPhysicalSensorType(SensorValueType value);
SensorValueType toSensorValueType(PhysicalSensorType value);

#endif /* ConversionHelper_hpp */
