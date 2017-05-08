//
//  LogicRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "logic/rules/LogicRule.hpp"

static const shared_ptr<string> DEFAULT_NAME = make_shared<string>("unnamed");

LogicRule::LogicRule()
: enabled(true), ruleId(0), name(DEFAULT_NAME) {
  
}

void LogicRule::setEnabled(bool state) {
  enabled = state;
}

bool LogicRule::isEnabled() {
  return enabled;
}

void LogicRule::setRuleId(int id) {
  ruleId = id;
}

int LogicRule::getRuleId() {
  return ruleId;
}

void LogicRule::setName(const shared_ptr<string> name) {
  this->name = name == nullptr ? DEFAULT_NAME : name;
}

shared_ptr<string> LogicRule::getName() {
  return name;
}
