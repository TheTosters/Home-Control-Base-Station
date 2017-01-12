//
//  LogicRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "LogicRule.hpp"

LogicRule::LogicRule()
: enabled(true) {
  
}

void LogicRule::setEnabled(bool state) {
  enabled = state;
}

bool LogicRule::isEnabled() {
  return enabled;
}
