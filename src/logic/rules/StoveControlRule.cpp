//
//  StoveControlRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "logic/rules/StoveControlRule.hpp"
#include "SharedStatesConsts.h"

StoveControlRule::StoveControlRule(SharedState state)
: sharedState(state) {
  
}

void StoveControlRule::execute(){
  if ((*sharedState)[STATE_WANT_HEATING] != 0) {
    //todo: transfer info to stove that we want heat!
  }
}
