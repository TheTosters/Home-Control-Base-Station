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
#include "logic/rules/LogicRule.hpp"
#include "logic/Logic.hpp"

class SetupSharedStateRule : public LogicRule {
  public:
    SetupSharedStateRule(SharedState state);
    virtual ~SetupSharedStateRule();
  
    virtual void execute() override;
    void setValue(int key, int value);
    void removeValue(int key);
    SharedState getValuesCopy();
  private:
    SharedState sharedState;
    SharedState valuesToSet;
};

#endif /* SetupSharedStateRule_hpp */
