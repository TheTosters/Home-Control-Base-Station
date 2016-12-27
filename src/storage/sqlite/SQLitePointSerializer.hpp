//
//  SQLitePointSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLitePointSerializer_hpp
#define SQLitePointSerializer_hpp

#include <stdio.h>
#include <string>

#include "SQLiteEntitySerializer.hpp"
#include "Point.hpp"
#include "SimpleCriteria.hpp"

using namespace std;

class Entity;

class SQLitePointSerializer : public SQLiteEntitySerializer {
  public:
    void store(Point* data);
    void storeOrUpdate(Point* data);
    void useDatabase(sqlite3 *db, Storage* storage);
    shared_ptr<vector<shared_ptr<Point>>> loadAll();
    shared_ptr<vector<shared_ptr<Point>>> loadMatching(SimpleCriteria criteria);
    shared_ptr<Point> load(long id);
};

#endif /* SQLitePointSerializer_hpp */
