/*
 * AcquisitorTask.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/AcquisitorTask.hpp>
#include "sensor-net/CommunicationLink.hpp"
#include "sensor-net/SensorNetProtocolParser.hpp"

static const int MAX_RETRY_ATTEMPTS = 10;

AcquisitorTask::AcquisitorTask(shared_ptr<PhysicalSensor> sensor, int count, SensorDataListener* listener,
    shared_ptr<spdlog::logger> logger)
: sensor(sensor), listener(listener), attempts(0), measurementsCount(count), logger(logger) {

}

int AcquisitorTask::getSensorId() {
  return sensor->getId();
}

bool AcquisitorTask::execute(){
  attempts ++;
  MeasurementMap measurements = nullptr;
  try {
    CommunicationLink link(cltBluetooth, sensor, logger);
    if (link.isConnected()) {
      SensorNetProtocolParser parser(&link);
      measurements = make_shared<unordered_map<string, MeasurementList>>();
      parser.requestMeasurement(measurements, measurementsCount);
      listener->onSensorData(sensor, measurements);
      return true;
    }

  } catch (std::exception const& e) {
    logger->error("Exception at fetchMeasurements:");
    logger->error(e.what());

  } catch (...) {
    logger->error("Undefined exception at fetchMeasurements");
  }
  logger->warn("Acquisition from {}({}) failed, we will retry (probably)", sensor->getName(), sensor->getAddress());
  return attempts > MAX_RETRY_ATTEMPTS;
}
