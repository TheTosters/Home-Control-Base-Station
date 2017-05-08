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
#include <memory>
#include <string>

using namespace std;

class LogicRule {
  public:
    LogicRule();
    virtual ~LogicRule() = default;
  
    void setEnabled(bool state);
    bool isEnabled();

    void setRuleId(int id);
    int getRuleId();

    void setName(const shared_ptr<string> name);
    shared_ptr<string> getName();

    virtual void execute() = 0;
  protected:
    bool  enabled;
    int ruleId;
    shared_ptr<string> name;
};

typedef vector<shared_ptr<LogicRule>> LogicRulesVector;
typedef shared_ptr<LogicRulesVector> LogicRulesList;

#endif /* LogicRule_hpp */
