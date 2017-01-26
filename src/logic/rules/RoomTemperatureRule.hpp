//
//  RoomTemperatureRule.hpp
//  HomeControl
//
//  Created by Bartłomiej on 23/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef RoomTemperatureRule_hpp
#define RoomTemperatureRule_hpp

#include <stdio.h>
#include "LogicRule.hpp"
#include "Logic.hpp"

class RoomTemperatureRule : public LogicRule {
public:
  RoomTemperatureRule(shared_ptr<Logic> logic);
  
private:
  shared_ptr<Logic> logic;
};

#endif /* RoomTemperatureRule_hpp */
