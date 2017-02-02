//
//  StoveControlRule.hpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef StoveControlRule_hpp
#define StoveControlRule_hpp

#include <stdio.h>
#include "logic/rules/LogicRule.hpp"
#include "logic/Logic.hpp"

class StoveControlRule : public LogicRule {
  public:
    StoveControlRule(SharedState state);
    virtual void execute() override;
  
  private:
    SharedState sharedState;
};

#endif /* StoveControlRule_hpp */
