//
//  TemperatureIdentifier.cpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "logic/TemperatureIdentifier.hpp"

TemperatureIdentifier::TemperatureIdentifier(string _name, double temp)
: name(_name), temperature(temp){
  
}

string const& TemperatureIdentifier::getName() const {
  return name;
}

double TemperatureIdentifier::getTemperature() {
  return temperature;
}
