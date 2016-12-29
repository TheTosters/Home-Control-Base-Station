//
//  SQLiteRoomSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 18/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <vector>
#include "SQLiteRoomSerializer.hpp"
#include "Room.hpp"
#include "SQLiteFillableStatement.hpp"
#include "Storage.hpp"
#include "Sensor.hpp"
#include "SQLiteSensorSerializer.hpp"
#include "SQLitePointSerializer.hpp"

void SQLiteRoomSerializer::store(Room* data) {
  Room* p = static_cast<Room*>(data);
  SQLiteFillableStatement statement(db, "INSERT INTO Rooms (NULL, ?, ?)");
  statement.bindNext(p->getName());
  statement.bindNext(p->getFloor());
  p->setId( statement.executeInsert() );
  
  //store sensors
  vector<Sensor>* tmp = p->getSensors();
  for(vector<Sensor>::iterator it = tmp->begin(); it != tmp->end(); it ++) {
    SQLiteSensorSerializer* helper = storage->requestSerializer<SQLiteSensorSerializer>(*it);
    helper->storeOrUpdate( &(*it) );
  }
  
  //store shape
  vector<Point>* tmp2 = p->getShape();
  for(vector<Point>::iterator it = tmp2->begin(); it != tmp2->end(); it ++) {
    SQLitePointSerializer* helper = storage->requestSerializer<SQLitePointSerializer>(*it);
    helper->storeOrUpdate( &(*it) );
  }
}

void SQLiteRoomSerializer::storeOrUpdate(Room* data) {
  if (data->getId() < 0) {
    store(data);
    return;
  }
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

bool SQLiteRoomSerializer::remove(long id, bool dropSensors) {
  
  int deletedRows;
  
  SQLiteFillableStatement statement(db, "DELETE FROM Rooms WHERE id=?");
  statement.bindNext(id);
  statement.executeUpdate(&deletedRows);
  
  SimpleCriteria criteria;
  criteria.helperId = id;
  
  if (dropSensors == true) {
    SQLiteSensorSerializer* sensorSerializer = storage->requestSerializer<SQLiteSensorSerializer>( Sensor() );
    sensorSerializer->remove(criteria);
  }
  
  SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>( Point() );
  pointSerializer->remove(criteria);
  
  return deletedRows > 0;
}

shared_ptr<vector<shared_ptr<Room>>> SQLiteRoomSerializer::loadAll() {
  shared_ptr<vector<shared_ptr<Room>>> result = make_shared<vector<shared_ptr<Room>>>();
  
  SQLiteFillableStatement statement(db, "SELECT * FROM Rooms");
  while(statement.executeSelectNext() > 0) {
    long id;
    int floor;
    string name;
    
    statement.getColumns( &id, &name, &floor);
    shared_ptr<Room> room = make_shared<Room>(id, name);
    room->setFloor(floor);
    
    SimpleCriteria criteria;
    criteria.helperId = id;
    
    SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>(Point());
    shared_ptr<vector<shared_ptr<Point>>> shape = pointSerializer->loadMatching(criteria);
    if (shape != nullptr) {
      vector<Point>* destShape = room->getShape();
      for(vector<shared_ptr<Point>>::iterator iter = shape->begin(); iter != shape->end(); iter++) {
        destShape->push_back( iter->get() );
      }
    }
    
    SQLiteSensorSerializer* sensorSerializer = storage->requestSerializer<SQLiteSensorSerializer>( Sensor() );
    shared_ptr<vector<shared_ptr<Sensor>>> sensors = sensorSerializer->loadMatching(criteria);
    if (sensors != nullptr) {
      vector<Sensor>* destSensors = room->getSensors();
      for(vector<shared_ptr<Sensor>>::iterator iter = sensors->begin(); iter != sensors->end(); iter++) {
        destSensors->push_back( *iter->get() );
      }
    }
    
    result->push_back(room);
  }
  return result;
}

shared_ptr<vector<shared_ptr<Room>>> SQLiteRoomSerializer::loadMatching(SimpleCriteria criteria) {
  shared_ptr<vector<shared_ptr<Room>>> result = make_shared<vector<shared_ptr<Room>>>();
  
  if (criteria.id >= 0) {
    result->push_back( load(criteria.id) );
    
  } else {
    //implement if needed, probably never will be used
    fprintf(stderr, "%s: Not implemented! \n", __PRETTY_FUNCTION__);
  }
  
  return result;
}

shared_ptr<Room> SQLiteRoomSerializer::load(long id) {
  SQLiteFillableStatement statement(db, "SELECT * FROM Points WHERE id=?");
  statement.bindNext(id);
  if (statement.executeSelectNext() > 0) {
    long id;
    int floor;
    string name;
    
    statement.getColumns( &id, &name, &floor);
    shared_ptr<Room> result = make_shared<Room>(id, name);
    result->setFloor(floor);
    
    SimpleCriteria criteria;
    criteria.helperId = id;
    
    SQLitePointSerializer* pointSerializer = storage->requestSerializer<SQLitePointSerializer>(Point());
    shared_ptr<vector<shared_ptr<Point>>> shape = pointSerializer->loadMatching(criteria);
    if (shape != nullptr) {
      vector<Point>* destShape = result->getShape();
      for(vector<shared_ptr<Point>>::iterator iter = shape->begin(); iter != shape->end(); iter++) {
        destShape->push_back( iter->get() );
      }
    }
    
    SQLiteSensorSerializer* sensorSerializer = storage->requestSerializer<SQLiteSensorSerializer>( Sensor() );
    shared_ptr<vector<shared_ptr<Sensor>>> sensors = sensorSerializer->loadMatching(criteria);
    if (sensors != nullptr) {
      vector<Sensor>* destSensors = result->getSensors();
      for(vector<shared_ptr<Sensor>>::iterator iter = sensors->begin(); iter != sensors->end(); iter++) {
        destSensors->push_back( *iter->get() );
      }
    }
    
    return result;
    
  } else {
    return nullptr;
  }
}