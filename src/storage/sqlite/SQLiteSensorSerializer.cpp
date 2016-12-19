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

void SQLiteSensorSerializer::store(Entity* data) {
  Sensor* p = static_cast<Sensor*>(data);
  
  storage->addOrUpdate(p->getPosition());
  
  SQLiteFillableStatement statement(db, "INSERT INTO Sensors (NULL, ?, ?, ?)");
  statement.bindNext(p->getPosition()->getId());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
  p->setId( statement.executeInsert() );
}

void SQLiteSensorSerializer::storeOrUpdate(Entity* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  Sensor* p = static_cast<Sensor*>(data);
  storage->addOrUpdate(p->getPosition());
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Sensors (id, positionId, name, roomId) VALUES (?, ?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getPosition()->getId());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
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
  )";
  executeUpdateQuery(creationSql);
}
