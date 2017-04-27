//
//  SetupSharedStateRule.hpp
//  HomeControl
//
//  Created by Bartłomiej on 23/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef SetupSharedStateRule_hpp
#define SetupSharedStateRule_hpp

#include <stdio.h>
#include <unordered_map>
#include "logic/rules/LogicRule.hpp"
#include "logic/Logic.hpp"

class SetupSharedStateRule : public LogicRule {
  public:
    SetupSharedStateRule(shared_ptr<SharedState> state);
    virtual ~SetupSharedStateRule() = default;
  
    virtual void execute() override;
    void setValue(string key, int value);
    void removeValue(string key);
    unordered_map<string, int> getValuesCopy();
  private:
    shared_ptr<SharedState> sharedState;
    unordered_map<string, int> valuesToSet;
};

#endif /* SetupSharedStateRule_hpp */
