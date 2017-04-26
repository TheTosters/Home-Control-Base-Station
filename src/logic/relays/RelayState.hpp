/*
 * RelayState.h
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#ifndef RelayState_hpp
#define RelayState_hpp

#include <memory>
#include "entities/PhysicalSensor.hpp"
#include <misc/SimpleActionListener.hpp>

using namespace std;

typedef enum {
  cdrsWaiting, cdrsDelivered, cdrsFailed
} CommandDelivereedRelayState;

typedef enum{
  rssctRequestState,
  rssctChangeInitialState
} RelayStateSentCommandType;

class RelayStateSentCommand {
  public:
    int commandId;
    bool requestedState;
    long long durationOfState;
    RelayStateSentCommandType type;
    CommandDelivereedRelayState commandDeliveredState;

    RelayStateSentCommand(int commandId, bool requestedState, long long durationOfState,
        RelayStateSentCommandType type, CommandDelivereedRelayState commandDeliveredState);
};

class RelayState : public SimpleActionListener {
  public:
    RelayState(int relayId, shared_ptr<PhysicalSensor> sensor, int sensorRelayIndex, bool initialState);
    virtual ~RelayState() = default;
    bool isRelayEnabled();
    long long timeToStateChange();
    void setRequestedState(int commandId, bool requestedState, long long duration);
    void setInitialState(int commandId, bool state);
    bool getInitialState();

    virtual void onActionSuccess(int id) override;
    virtual void onActionError(int id, int error) override;
    virtual void onActionFailure(int id) override;
  private:
    int relayId;  //this relay state id
    shared_ptr<PhysicalSensor> sensor;
    int         sensorRelayIndex;       //each sensor can have several relays, which one it is?
    long long   timeOfChange;   //in ms from epoc (from moment when commandDelivered is true)
    bool        initialState;   //from this state sensor will be changed, and it will return to this state after durationOfState
    bool        requestedState; //this state will be executed for durationOfState seconds
    long long   durationOfState;  //after this time, relay will be moved to opposite state
    vector<RelayStateSentCommand> commandsToTrace;

    void applyCommand(RelayStateSentCommand& cmd);
};

#endif /* RelayState_hpp */
