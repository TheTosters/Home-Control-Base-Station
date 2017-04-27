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
#include "logic/rules/LogicRule.hpp"
#include "logic/Logic.hpp"

class RoomTemperatureRule : public LogicRule {
  public:
    RoomTemperatureRule(shared_ptr<Logic> logic, string stoveStateName);
    virtual void execute() override;
  private:
    shared_ptr<Logic> logic;
    string stoveStateName;
  
    double getTemperatureInRoom(shared_ptr<Room> room, bool& measured);
};

#endif /* RoomTemperatureRule_hpp */
