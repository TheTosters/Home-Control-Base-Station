/*
 * DataAcquisitor.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Zarnowski
 */

#include <sensor-net/DataAcquisitor.hpp>
#include <sensor-net/tasks/FetchMeasurementTask.hpp>
#include <algorithm>
#include <sensor-net/tasks/SendSimpleCommandTask.hpp>

DataAcquisitor::DataAcquisitor(shared_ptr<spdlog::logger> logger)
: workerThread(nullptr), logger(logger), paused(false) {

}

DataAcquisitor::~DataAcquisitor() {
  stopThread();
}

void DataAcquisitor::stopThread() {
  logger->info("Stopping worker thread");
  unique_lock<mutex> lock(innerMutex);
  workerThread = nullptr; //delete inside thread main
}

void DataAcquisitor::startThread() {
  logger->info("Starting worker thread");
  unique_lock<mutex> lock(innerMutex);
  if (workerThread != nullptr) {
    logger->warn("Thread already started");
    return;
  }
  if (paused) {
    logger->error("Can't start thread while in paused state!");
    return;
  }

  workerThread = new thread(&DataAcquisitor::workerThreadMain, this);
}

void DataAcquisitor::workerThreadMain() {
  logger->info("{} Worker thread main loop.", __func__);

  unique_lock<mutex> lock(innerMutex);
  thread* threadPtr = workerThread;
  lock.unlock();

  while(true) {
    try{
      //check exit conditions
      lock.lock();
      if ((workerThread == nullptr) || (paused == true) || (tasksToExecute.size() == 0)) {
        //we don't want to release lock here! It will be released after we cleanup workerThread field!
        break;
      }
      shared_ptr<AcquisitorTask> task = tasksToExecute.front();
      tasksToExecute.erase(tasksToExecute.begin());
      lock.unlock();

      if (task->execute() == false) {
        lock.lock();
        tasksToExecute.push_back(task);
        lock.unlock();
      }
    } catch (...) {
      printf(" --- catch\n");
    }
  }
  logger->info("{} Worker thread exit main loop.", __func__);
  //release thread memory
  //NOTE: we should be already in locked mutex state here!
  threadPtr->detach();  //don't use workerThread here, it might be nullptr
  delete threadPtr;
  workerThread = nullptr;
  lock.unlock();

  logger->info("{} Worker thread done.", __func__);
}

void DataAcquisitor::fetch(shared_ptr<PhysicalSensor> sensor, SensorDataListener* listener, int count) {
  unique_lock<mutex> lock(innerMutex);
  auto item = find_if(tasksToExecute.begin(), tasksToExecute.end(), [=](shared_ptr<AcquisitorTask> task) {
    return task->getSensorId() == sensor->getId();
  } );

  if (item != tasksToExecute.end()) {
    logger->warn("Sensor {}({}) already in fetch list, not added again.", sensor->getName(), sensor->getAddress());
    if (workerThread == nullptr && paused == false) {
      lock.unlock();
      startThread();
    } else {
      logger->warn("Thread already started?");
    }
    return;
  }

  logger->info("Adding sensor {}({}) to fetch queue.", sensor->getName(), sensor->getAddress());
  tasksToExecute.push_back( make_shared<FetchMeasurementTask>(sensor, count, listener, logger));

  if (workerThread == nullptr && paused == false) {
    lock.unlock();
    startThread();
  }
}

int DataAcquisitor::sendSimpleCommand(shared_ptr<PhysicalSensor> sensor, shared_ptr<string> command,
    NumbersList argList, shared_ptr<SimpleActionListener> listener) {

  unique_lock<mutex> lock(innerMutex);

  shared_ptr<SendSimpleCommandTask> task = make_shared<SendSimpleCommandTask>(sensor, command, argList, listener,
      logger);
  logger->info("Adding send simple command {} to sensor {}({}).", *command, sensor->getName(), sensor->getAddress());
  tasksToExecute.push_back(task);

  if (workerThread == nullptr && paused == false) {
    lock.unlock();
    startThread();
  }
  return task->getTaskId();
}

void DataAcquisitor::pause(bool waitForPause) {
  logger->info("DataAcquisitor pause operations");
  {
    //critical section
    unique_lock<mutex> lock(innerMutex);
    paused = true;
  }
  while (waitForPause && (isPaused() == false)) {
    usleep(50 * 1000);
  }
}

bool DataAcquisitor::isPaused() {
  unique_lock<mutex> lock(innerMutex);
  return paused && (workerThread == nullptr);
}

void DataAcquisitor::resume() {
  logger->info("DataAcquisitor resume normal operations");
  unique_lock<mutex> lock(innerMutex);
  paused = false;
  if (tasksToExecute.size() > 0) {
    lock.unlock();
    startThread();
  }
}
