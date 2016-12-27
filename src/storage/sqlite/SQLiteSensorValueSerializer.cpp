//
//  SQLiteSensorValueSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 27/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteSensorValueSerializer.hpp"

void SQLiteSensorValueSerializer::store(SensorValue* data) {
  //todo: implement
}

void SQLiteSensorValueSerializer::storeOrUpdate(SensorValue* data) {
  //todo: implement
}

void SQLiteSensorValueSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  //todo: implment
}

shared_ptr<vector<shared_ptr<SensorValue>>> SQLiteSensorValueSerializer::loadAll() {
  //todo: implement
  return nullptr;
}

shared_ptr<vector<shared_ptr<SensorValue>>> SQLiteSensorValueSerializer::loadMatching(SimpleCriteria criteria) {
  //todo: implement
  return nullptr;
}

shared_ptr<SensorValue> SQLiteSensorValueSerializer::load(long id) {
  //todo: implement
  return nullptr;
}
