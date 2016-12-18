//
//  SQLitePointSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLitePointSerializer_hpp
#define SQLitePointSerializer_hpp

#include "SQLiteEntitySerializer.hpp"

#include <stdio.h>
#include <string>

using namespace std;

class Entity;

class SQLitePointSerializer : protected SQLiteEntitySerializer {
  public:
    virtual void store(Entity* data) override;
    virtual void storeOrUpdate(Entity* data) override;
    virtual void useDatabase(sqlite3 *db, Storage* storage) override;
};

#endif /* SQLitePointSerializer_hpp */
