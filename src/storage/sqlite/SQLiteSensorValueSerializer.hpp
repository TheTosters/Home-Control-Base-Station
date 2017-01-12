//
//  SQLiteSensorValueSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 27/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteSensorValueSerializer_hpp
#define SQLiteSensorValueSerializer_hpp

#include <stdio.h>
#include <vector>

#include "SimpleCriteria.hpp"
#include "SQLiteEntitySerializer.hpp"
#include "SensorValue.hpp"
#include "SQLiteFillableStatement.hpp"

class SQLiteSensorValueSerializer : public SQLiteEntitySerializer {
  public:
    void store(SensorValue* data);
    void storeOrUpdate(SensorValue* data);
    void useDatabase(sqlite3 *db, Storage* storage);
    
    SensorValueList loadAll();
    SensorValueList loadMatching(SimpleCriteria const& criteria);
    shared_ptr<SensorValue> load(long id);
  
    bool remove(SimpleCriteria const& criteria);
    long count(SimpleCriteria const& criteria);
  private:
    void append(string& sql, bool condition, const string& toAppend, bool &andMarker);
    shared_ptr<SensorValue> deserializeRow(SQLiteFillableStatement& statement);
  
    shared_ptr<SQLiteFillableStatement> appendWhere(string& sql, SimpleCriteria const& criteria);
};

#endif /* SQLiteSensorValueSerializer_hpp */
