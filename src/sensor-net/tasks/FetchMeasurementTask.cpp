/*
 * FetchMeasurementTask.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/tasks/FetchMeasurementTask.hpp>
#include "sensor-net/CommunicationLink.hpp"
#include "sensor-net/SensorNetProtocolParser.hpp"

FetchMeasurementTask::FetchMeasurementTask(shared_ptr<PhysicalSensor> sensor, int count, SensorDataListener* listener,
    shared_ptr<spdlog::logger> logger)
: AcquisitorTask(sensor, logger), listener(listener), measurementsCount(count) {

}

bool FetchMeasurementTask::innerExecute() {
  MeasurementMap measurements = nullptr;
  try {
    CommunicationLink link(cltBluetooth, sensor, logger);
    if (link.isConnected()) {
      SensorNetProtocolParser parser(&link);
      measurements = make_shared<unordered_map<string, MeasurementList>>();
      parser.requestMeasurement(measurements, measurementsCount);
      listener->onSensorData(sensor, measurements);
      printf("[FetchMeasurementTask task %p] Exit acquisition task\n", this);
      return true;
    }

  } catch (std::exception const& e) {
    logger->error("Exception at FetchMeasurementTask");
    //logger->error(e.what());  //hmm this cause thread crash :(

  } catch (...) {
    logger->error("Undefined exception at FetchMeasurementTask");
  }
  return false;
}
