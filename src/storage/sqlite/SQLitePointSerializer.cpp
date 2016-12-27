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

shared_ptr<vector<shared_ptr<Point>>> SQLitePointSerializer::loadAll() {
  //implement if needed, probably never will be used
  fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  return make_shared<vector<shared_ptr<Point>>>();
}

shared_ptr<vector<shared_ptr<Point>>> SQLitePointSerializer::loadMatching(SimpleCriteria criteria) {
  shared_ptr<vector<shared_ptr<Point>>> result = make_shared<vector<shared_ptr<Point>>>();
  if (criteria.id >= 0) {
    result->push_back( load(criteria.id) );
  
  } else {
    //implement if needed, probably never will be used
    fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  }
  
  return result;
}

shared_ptr<Point> SQLitePointSerializer::load(long id) {
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