//
//  StoveControlRule.cpp
//  HomeControl
//
//  Created by Bartłomiej on 12/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include <logic/rules/TransferStateToRelayRule.hpp>
#include "SharedStatesConsts.h"

TransferStateToRelayRule::TransferStateToRelayRule(shared_ptr<SharedState> sharedState,
    shared_ptr<RelaysStatesMachine> stateMachine, string stateToTransfer, int relayId, int duration)
: sharedState(sharedState), relayId(relayId), duration(duration), stateToTransfer(stateToTransfer),
  stateMachine(stateMachine) {
  
}

void TransferStateToRelayRule::execute(){
  int relayState = (*sharedState)[stateToTransfer];
  stateMachine->demandRelayState(relayId, relayState != 0, duration);
}
