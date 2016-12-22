//
//  SQLitePointSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLitePointSerializer.hpp"
#include "Point.hpp"
#include "SQLiteFillableStatement.hpp"

void SQLitePointSerializer::store(Entity* data) {
  Point* p = static_cast<Point*>(data);
  SQLiteFillableStatement statement(db, "INSERT INTO Points (NULL, ?, ?)");
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  p->setId( statement.executeInsert() );
}

void SQLitePointSerializer::storeOrUpdate(Entity* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  Point* p = static_cast<Point*>(data);
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Points (id, x, y) VALUES (?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  statement.executeUpdate();
}

void SQLitePointSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS Points (\
      id INT PRIMARY KEY NOT NULL,  \
      x INT NOT NULL,               \
      y INT NOT NULL,               \
    )";
  executeUpdateQuery(creationSql);
}

void SQLitePointSerializer::loadAll(shared_ptr<vector<shared_ptr<Entity>>> result) {
  //TODO:
}

void SQLitePointSerializer::loadMatching(SimpleCriteria criteria, shared_ptr<vector<shared_ptr<Entity>>> result) {
  //TODO:
}

shared_ptr<Entity> SQLitePointSerializer::load(long id) {
  SQLiteFillableStatement statement(db, "SELECT * FROM Points WHERE id=?");
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    long id;
    int x,y;
    
    statement.getColumns( &id, &x, &y);
    return make_shared<Point>(id, x, y);
    
  } else {
    return nullptr;
  }
}