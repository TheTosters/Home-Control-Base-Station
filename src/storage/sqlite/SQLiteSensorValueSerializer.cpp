//
//  SQLiteSensorValueSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 27/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteSensorValueSerializer.hpp"
#include "SQLiteFillableStatement.hpp"

void SQLiteSensorValueSerializer::store(SensorValue* data) {
  SQLiteFillableStatement statement(db, "INSERT INTO SensorValues VALUES (NULL, ?, ?, ?, ?)");
  statement.bindNext(data->getPhysicalSensorId());
  statement.bindNext(data->getValue());
  statement.bindNext( static_cast<int>(data->getValueType()) );
  statement.bindNext( static_cast<long>( data->getTimestamp() ));
  data->setId( statement.executeInsert() );
}

void SQLiteSensorValueSerializer::storeOrUpdate(SensorValue* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
  
  SQLiteFillableStatement statement(db, "INSERT OR REPLACE INTO SensorValues (id, physicalSensorId, value, valueType, timestamp) VALUES (?, ?, ?, ?, ?)");
  statement.bindNext(data->getId());
  statement.bindNext(data->getPhysicalSensorId());
  statement.bindNext(data->getValue());
  statement.bindNext( static_cast<int>(data->getValueType()) );
  statement.bindNext( static_cast<long>( data->getTimestamp() ));
  statement.executeUpdate();
}

void SQLiteSensorValueSerializer::useDatabase(sqlite3 *db, Storage* storage) {
  SQLiteEntitySerializer::useDatabase(db, storage);
  
  //create table if needed
  string creationSql = "CREATE TABLE IF NOT EXISTS SensorValues (\
    id INTEGER PRIMARY KEY, \
    physicalSensorId INT NOT NULL, \
    value REAL NOT NULL, \
    valueType INT NOT NULL \
    timestamp INT NOT NULL \
  )";
  executeUpdateQuery(creationSql);
}

shared_ptr<vector<shared_ptr<SensorValue>>> SQLiteSensorValueSerializer::loadAll() {
  //todo: implement when needed
  return nullptr;
}

void SQLiteSensorValueSerializer::append(string& sql, bool condition, const string& toAppend, bool &andMarker) {
  if (condition == false) {
    return;
  }
  if (andMarker) {
    sql.append(" AND ");
  }
  andMarker = true;
  sql.append(toAppend);
}

shared_ptr<SensorValue> SQLiteSensorValueSerializer::deserializeRow(SQLiteFillableStatement& statement) {
  long id, sensId, timestamp;
  int valType;
  double value;
  
  statement.getColumns( &id, &sensId, &value, &valType, &timestamp);
  
  return make_shared<SensorValue>(id, sensId, value, static_cast<SensorValueType>(valType), static_cast<time_t>(timestamp));
}

shared_ptr<vector<shared_ptr<SensorValue>>> SQLiteSensorValueSerializer::loadMatching(SimpleCriteria criteria) {

  bool andRequired = false;
  string sql("SELECT * FROM SensorValues WHERE ");
  
  append(sql, criteria.from > 0, "(timestamp >= ?)", andRequired);
  append(sql, criteria.to > 0, "(timestamp <= ?)", andRequired);
  
  andRequired = false;
  append(sql, criteria.count > 0, " LIMIT ?", andRequired);
  
  andRequired = false;
  append(sql, criteria.offset > 0, " OFFSET ?", andRequired);
  
  //build statement
  SQLiteFillableStatement statement(db, sql);
  
  //bind data, NOTE: order is important!
  if (criteria.from > 0) {
    statement.bindNext( static_cast<long>(criteria.from) );
  }
  if (criteria.to > 0) {
    statement.bindNext( static_cast<long>(criteria.to) );
  }
  if (criteria.count > 0) {
    statement.bindNext( criteria.count );
  }
  if (criteria.offset > 0) {
    statement.bindNext( criteria.offset );
  }
  
  shared_ptr<vector<shared_ptr<SensorValue>>> result = make_shared<vector<shared_ptr<SensorValue>>>();
  
  while(statement.executeSelectNext() > 0) {
    result->push_back(deserializeRow(statement));
  }
  
  return result;
}

shared_ptr<SensorValue> SQLiteSensorValueSerializer::load(long id) {
  SQLiteFillableStatement statement(db, "SELECT * FROM SensorValues WHERE id=?");
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    return deserializeRow(statement);
    
  } else {
    return nullptr;
  }
}
