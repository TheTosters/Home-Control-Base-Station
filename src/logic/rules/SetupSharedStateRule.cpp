//
//  SetupSharedStateRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 23/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "logic/rules/SetupSharedStateRule.hpp"
#include <algorithm>

SetupSharedStateRule::SetupSharedStateRule(shared_ptr<SharedState> state)
: sharedState(state) {
  
}

void SetupSharedStateRule::execute() {
  for(auto iter = valuesToSet.begin(); iter != valuesToSet.end(); iter++) {
    (*sharedState)[iter->first] = iter->second;
  }
}

void SetupSharedStateRule::setValue(string key, int value) {
  valuesToSet[key] = value;
}

void SetupSharedStateRule::removeValue(string key) {
  valuesToSet.erase(key);
}

unordered_map<string, int> SetupSharedStateRule::getValuesCopy() {
  return valuesToSet;
}
