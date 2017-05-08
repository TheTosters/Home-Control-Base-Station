//
//  StoveControlRule.hpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef TransferStateToRelayRule_hpp
#define TransferStateToRelayRule_hpp

#include <stdio.h>
#include "logic/rules/LogicRule.hpp"
#include "logic/Logic.hpp"
#include <logic/relays/RelaysStatesMachine.hpp>

class TransferStateToRelayRule : public LogicRule {
  public:
    TransferStateToRelayRule(shared_ptr<SharedState> sharedState, shared_ptr<RelaysStatesMachine> stateMachine,
        string stateToTransfer, int relayId, int duration);
    virtual void execute() override;
  private:
    shared_ptr<SharedState> sharedState;
    int relayId;
    int duration;
    string stateToTransfer;
    shared_ptr<RelaysStatesMachine> stateMachine;
};

#endif /* TransferStateToRelayRule_hpp */
