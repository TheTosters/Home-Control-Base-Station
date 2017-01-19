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
#include "LogicRule.hpp"
#include "Logic.hpp"

class StoveControlRule : public LogicRule {
  public:
    StoveControlRule(shared_ptr<Logic> logic);
  
  private:
    shared_ptr<Logic> logic;
};

#endif /* StoveControlRule_hpp */
