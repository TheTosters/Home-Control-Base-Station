//
//  LogicRule.hpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef LogicRule_hpp
#define LogicRule_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class LogicRule {
  public:
    virtual void execute() = 0;
};

typedef vector<shared_ptr<LogicRule>> LogicRulesList;

#endif /* LogicRule_hpp */
