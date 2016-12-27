//
//  SQLiteSensorSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 18/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteSensorSerializer.hpp"
#include "Sensor.hpp"
#include "SQLiteFillableStatement.hpp"
#include "Storage.hpp"
#include "SQLitePointSerializer.hpp"

void SQLiteSensorSerializer::store(Sensor* data) {
  Sensor* p = static_cast<Sensor*>(data);
  
  SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>(Point());
  pointSerializer->store(p->getPosition());
  
  SQLiteFillableStatement statement(db, "INSERT INTO Sensors (NULL, ?, ?, ?, ?)");
  statement.bindNext(p->getPosition()->getId());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
  statement.bindNext(p->getAddress());
  p->setId( statement.executeInsert() );
}

void SQLiteSensorSerializer::storeOrUpdate(Sensor* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  Sensor* p = static_cast<Sensor*>(data);
  SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>(Point());
  pointSerializer->storeOrUpdate(p->getPosition());
  
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Sensors (id, positionId, name, roomId, address) VALUES (?, ?, ?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getPosition()->getId());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
  statement.bindNext(p->getAddress());
  statement.executeUpdate();
}

void SQLiteSensorSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS Sensors (\
    id INT PRIMARY KEY NOT NULL,  \
    positionId INT NOT NULL,      \
    name TEXT NOT NULL,           \
    roomId INT NOT NULL           \
    address TEXT                  \
  )";
  executeUpdateQuery(creationSql);
}

shared_ptr<vector<shared_ptr<Sensor>>> SQLiteSensorSerializer::loadAll() {
  //todo:implement when needed
  return nullptr;
}

shared_ptr<vector<shared_ptr<Sensor>>> SQLiteSensorSerializer::loadMatching(SimpleCriteria criteria) {
  //todo:implement when needed
  return nullptr;
}

shared_ptr<Sensor> SQLiteSensorSerializer::load(long id) {
  SQLiteFillableStatement statement(db, "SELECT * FROM Sensors WHERE id=?");
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    long id, positionId, roomId;
    string name, address;
    
    statement.getColumns( &id, &positionId, &name, &roomId, &address);

    SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>(Point());
    shared_ptr<Point> pos = pointSerializer->load(positionId);
    
    shared_ptr<Sensor> result = make_shared<Sensor>(id, name, pos.get());
    result->setRoomId(roomId);
    result->setAddress(address);
    
    return result;
    
  } else {
    return nullptr;
  }
}
