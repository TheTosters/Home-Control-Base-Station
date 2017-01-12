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
    LogicRule();
  
    void setEnabled(bool state);
    bool isEnabled();

    virtual void execute() = 0;
  protected:
    bool  enabled;
};

typedef vector<shared_ptr<LogicRule>> LogicRulesVector;
typedef shared_ptr<LogicRulesVector> LogicRulesList;

#endif /* LogicRule_hpp */
