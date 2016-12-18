//
//  SQLiteRoomSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 18/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteRoomSerializer.hpp"
#include "Room.hpp"
#include "SQLiteFillableStatement.hpp"

void SQLiteRoomSerializer::store(Entity* data) {
  Room* p = static_cast<Room*>(data);
  SQLiteFillableStatement statement(db, "INSERT INTO Rooms (NULL, ?, ?)");
  statement.bindNext(p->getName());
  statement.bindNext(p->getFloor());
  p->setId( statement.executeInsert() );
}

void SQLiteRoomSerializer::storeOrUpdate(Entity* data) {
  Room* p = static_cast<Room*>(data);
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Rooms (id, name, floor) VALUES (?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getName());
  statement.bindNext(p->getFloor());
  statement.executeUpdate();
}

void SQLiteRoomSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS Rooms (\
    id INT PRIMARY KEY NOT NULL,  \
    name TEXT NOT NULL,           \
    floor INT NOT NULL,           \
  )";
  executeUpdateQuery(creationSql);
}
