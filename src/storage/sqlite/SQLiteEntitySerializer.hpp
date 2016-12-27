//
//  SQLiteEntitySerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteEntitySerializer_hpp
#define SQLiteEntitySerializer_hpp

#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include "EntitySerializer.hpp"

using namespace std;

class Storage;

class SQLiteEntitySerializer : public EntitySerializer {
  public:
    virtual void useDatabase(sqlite3 *db, Storage* storage);
  protected:
    sqlite3*  db;
    Storage*  storage;

    bool executeUpdateQuery(string creationSql);
};

#endif /* SQLiteEntitySerializer_hpp */
