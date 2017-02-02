//
//  SetupSharedStateRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 23/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "logic/rules/SetupSharedStateRule.hpp"

SetupSharedStateRule::SetupSharedStateRule(SharedState state)
: sharedState(state),
  valuesToSet(make_shared<unordered_map<int, int>>()) {
  
}

SetupSharedStateRule::~SetupSharedStateRule() {
  
}

void SetupSharedStateRule::execute() {
  for(auto iter = valuesToSet->begin(); iter != valuesToSet->end(); iter++) {
    (*sharedState)[iter->first] = iter->second;
  }
}

void SetupSharedStateRule::setValue(int key, int value) {
  (*valuesToSet)[key] = value;
}

void SetupSharedStateRule::removeValue(int key) {
  valuesToSet->erase(key);
}

SharedState SetupSharedStateRule::getValuesCopy() {
  auto result = make_shared<unordered_map<int, int>>();
  result->insert(valuesToSet->begin(), valuesToSet->end());
  return result;
}
