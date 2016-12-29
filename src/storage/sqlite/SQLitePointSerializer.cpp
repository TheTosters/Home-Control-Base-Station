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

void SQLitePointSerializer::store(Point* data) {
  Point* p = static_cast<Point*>(data);
  SQLiteFillableStatement statement(db, "INSERT INTO Points VALUES (NULL, ?, ?, ?)");
  statement.bindNext(p->getOwnerId());
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  p->setId( (int)statement.executeInsert() );
}

void SQLitePointSerializer::storeOrUpdate(Point* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  Point* p = static_cast<Point*>(data);
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Points (id, ownerId, x, y) VALUES (?, ?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getOwnerId());
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  statement.executeUpdate();
}

void SQLitePointSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS Points (\
      id INTEGER PRIMARY KEY, \
      ownerId INT NOT NULL, \
      x INT NOT NULL, \
      y INT NOT NULL \
    )";
  executeUpdateQuery(creationSql);
}

shared_ptr<vector<shared_ptr<Point>>> SQLitePointSerializer::loadAll() {
  //implement if needed, probably never will be used
  fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  return make_shared<vector<shared_ptr<Point>>>();
}

shared_ptr<Point> SQLitePointSerializer::deserializeRow(SQLiteFillableStatement& statement) {
  long id, ownerId;
  int x, y;
  
  statement.getColumns( &id, &ownerId, &x, &y);
  auto point = make_shared<Point>(id, x, y);
  point->setOwnerId(ownerId);
  
  return point;
}

shared_ptr<vector<shared_ptr<Point>>> SQLitePointSerializer::loadMatching(SimpleCriteria criteria) {
  shared_ptr<vector<shared_ptr<Point>>> result = make_shared<vector<shared_ptr<Point>>>();
  if (criteria.id >= 0) {
    result->push_back( load(criteria.id) );
  
  } else if (criteria.helperId >= 0) {
    SQLiteFillableStatement statement(db, "SELECT * FROM Points WHERE ownerId=?");
    statement.bindNext(criteria.helperId);
    while(statement.executeSelectNext() > 0) {
      result->push_back( deserializeRow(statement) );
    }

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
    return deserializeRow(statement);
    
  } else {
    return nullptr;
  }
}

void SQLitePointSerializer::remove(long id) {
  SQLiteFillableStatement statement(db, "DELETE FROM Points WHERE id=?");
  statement.bindNext(id);
  statement.executeUpdate();
}

void SQLitePointSerializer::remove(SimpleCriteria criteria) {
  if (criteria.id >= 0) {
    remove(criteria.id);
    
  } else if (criteria.helperId >= 0) {
    SQLiteFillableStatement statement(db, "DELETE FROM Points WHERE ownerId=?");
    statement.bindNext(criteria.helperId);
    statement.executeUpdate();
    
  } else {
    //implement if needed, probably never will be used
    fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  }
}
