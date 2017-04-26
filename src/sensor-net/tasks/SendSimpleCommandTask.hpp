/*
 * SendSimpleCommandTask.hpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#ifndef SendSimpleCommandTask_hpp
#define SendSimpleCommandTask_hpp

#include <sensor-net/tasks/AcquisitorTask.hpp>
#include <sensor-net/parsers/Number.hpp>
#include <misc/SimpleActionListener.hpp>

class SendSimpleCommandTask: public AcquisitorTask {
  public:
    SendSimpleCommandTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<string> command, NumbersList argList,
        shared_ptr<SimpleActionListener> listener, shared_ptr<spdlog::logger> logger);
    virtual ~SendSimpleCommandTask() = default;

  protected:
    shared_ptr<string> command;
    NumbersList argList;
    shared_ptr<SimpleActionListener> listener;

    virtual bool innerExecute() override;
    virtual void giveUp() override;
};

#endif /* SendSimpleCommandTask_hpp */
