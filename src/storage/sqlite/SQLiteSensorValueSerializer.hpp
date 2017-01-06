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
    
    shared_ptr<vector<shared_ptr<SensorValue>>> loadAll();
    shared_ptr<vector<shared_ptr<SensorValue>>> loadMatching(SimpleCriteria criteria);
    shared_ptr<SensorValue> load(long id);
  private:
    void append(string& sql, bool condition, const string& toAppend, bool &andMarker);
    shared_ptr<SensorValue> deserializeRow(SQLiteFillableStatement& statement);
};

#endif /* SQLiteSensorValueSerializer_hpp */
