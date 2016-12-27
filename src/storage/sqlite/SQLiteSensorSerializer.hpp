//
//  SQLiteSensorSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 18/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteSensorSerializer_hpp
#define SQLiteSensorSerializer_hpp

#include <stdio.h>
#include "SQLiteEntitySerializer.hpp"
#include "Sensor.hpp"
#include "SimpleCriteria.hpp"

class SQLiteSensorSerializer : public SQLiteEntitySerializer {
  public:
    void store(Entity* data);
    void storeOrUpdate(Entity* data);
    void useDatabase(sqlite3 *db, Storage* storage);
  
    shared_ptr<vector<shared_ptr<Sensor>>> loadAll();
    shared_ptr<vector<shared_ptr<Sensor>>> loadMatching(SimpleCriteria criteria);
    shared_ptr<Sensor> load(long id);
};

#endif /* SQLiteSensorSerializer_hpp */
