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

void SQLiteSensorSerializer::store(Sensor* data) {
  Sensor* p = static_cast<Sensor*>(data);
  
  SQLiteFillableStatement statement(db, "INSERT INTO Sensors VALUES (NULL, ?, ?, ?, ?, ?, ?)");
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
  statement.bindNext(p->getAddress());
  statement.bindNext(p->getType());
  p->setId( statement.executeInsert() );
}

void SQLiteSensorSerializer::storeOrUpdate(Sensor* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  Sensor* p = static_cast<Sensor*>(data);
  
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO Sensors (id, x, y, name, roomId, address, type) VALUES (?, ?, ?, ?, ?, ?, ?)");
  statement.bindNext(p->getId());
  statement.bindNext(p->getX());
  statement.bindNext(p->getY());
  statement.bindNext(p->getName());
  statement.bindNext(p->getRoomId());
  statement.bindNext(p->getAddress());
  statement.bindNext(p->getType());
  statement.executeUpdate();
}

void SQLiteSensorSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS Sensors (\
    id INTEGER PRIMARY KEY, \
    x INT NOT NULL, \
    y INT NOT NULL, \
    name TEXT NOT NULL, \
    roomId INT NOT NULL, \
    address TEXT, \
    type TEXT \
  )";
  executeUpdateQuery(creationSql);
}

shared_ptr<vector<shared_ptr<Sensor>>> SQLiteSensorSerializer::loadAll() {
  //todo:implement when needed
  return nullptr;
}

shared_ptr<vector<shared_ptr<Sensor>>> SQLiteSensorSerializer::loadMatching(SimpleCriteria criteria) {
  shared_ptr<vector<shared_ptr<Sensor>>> result = make_shared<vector<shared_ptr<Sensor>>>();
  if (criteria.id >= 0) {
    result->push_back( load(criteria.id) );
    
  } else if (criteria.helperId >= 0) {
    SQLiteFillableStatement statement(db, "SELECT * FROM Sensors WHERE roomId=?");
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

shared_ptr<Sensor> SQLiteSensorSerializer::deserializeRow(SQLiteFillableStatement& statement) {
  long id, roomId;
  int x, y;
  string name, address, type;
  
  statement.getColumns( &id, &x, &y, &name, &roomId, &address, &type);
  
  shared_ptr<Sensor> result = make_shared<Sensor>(id);
  result->setName(name);
  result->setX(x);
  result->setY(y);
  result->setRoomId(roomId);
  result->setAddress(address);
  result->setType(type);
  
  return result;
}

shared_ptr<Sensor> SQLiteSensorSerializer::load(long id) {
  SQLiteFillableStatement statement(db, "SELECT * FROM Sensors WHERE id=?");
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    return deserializeRow(statement);
    
  } else {
    return nullptr;
  }
}

void SQLiteSensorSerializer::remove(long id) {
  SQLiteFillableStatement statement(db, "DELETE FROM Sensors WHERE id=?");
  statement.bindNext(id);
  statement.executeUpdate();
}

void SQLiteSensorSerializer::remove(SimpleCriteria criteria) {
  if (criteria.id >= 0) {
    remove(criteria.id);
    
  } else if (criteria.helperId >= 0) {
    SQLiteFillableStatement statement(db, "DELETE FROM Sensors WHERE roomId=?");
    statement.bindNext(criteria.helperId);
    statement.executeUpdate();
    
  } else {
    //implement if needed, probably never will be used
    fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  }
}
