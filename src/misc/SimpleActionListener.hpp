/*
 * SimpleActionListener.hpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Zarnowski
 */

#ifndef SimpleActionListener_hpp
#define SimpleActionListener_hpp

#include <sensor-net/parsers/Number.hpp>

class SimpleActionListener {
  public:
    virtual ~SimpleActionListener() = default;

    //Action was executed successfully
    virtual void onActionSuccess(int id, Number result) = 0;

    //Action was executed, but while execution error was reported
    virtual void onActionError(int id, int error) = 0;

    //Unable to execute action
    virtual void onActionFailure(int id) = 0;
};

#endif /* SimpleActionListener_hpp */
