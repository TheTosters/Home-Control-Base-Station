/*
 * SendSimpleCommandTask.hpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#ifndef SendSimpleCommandTask_hpp
#define SendSimpleCommandTask_hpp

#include <sensor-net/tasks/AcquisitorTask.hpp>

class SendSimpleCommandTask: public AcquisitorTask {
  public:
    SendSimpleCommandTask(shared_ptr<PhysicalSensor> sensor, shared_ptr<spdlog::logger> logger);
    virtual ~SendSimpleCommandTask() = default;

  protected:
    virtual bool innerExecute() override;
    virtual void giveUp() override;
};

#endif /* SendSimpleCommandTask_hpp */
