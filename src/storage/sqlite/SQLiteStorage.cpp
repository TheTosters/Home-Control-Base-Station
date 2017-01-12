//
//  SQLiteStorage.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteStorage.hpp"
#include "SQLiteEntitySerializer.hpp"
#include "Entity.hpp"

bool SQLiteStorage::open() {
  int rc = sqlite3_open_v2("storage.sqlite", &db, SQLITE_OPEN_CREATE|SQLITE_OPEN_READWRITE|SQLITE_OPEN_FULLMUTEX, NULL);
  if ( rc ){
    //todo: proper logs not fprint
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
    return false;
  }
  map<int, EntitySerializer*>::iterator iter = serializers.begin();
  while(iter != serializers.end()) {
    SQLiteEntitySerializer* sqlSer = static_cast<SQLiteEntitySerializer*>(iter->second);
    sqlSer->useDatabase(db, this);
    iter++;
  }
  return true;
}

void SQLiteStorage::close() {
  //todo: proper logs
  if (db != NULL) {
    sqlite3_close(db);
    db = NULL;

    map<int, EntitySerializer*>::iterator iter = serializers.begin();
    while(iter != serializers.end()) {
      SQLiteEntitySerializer* sqlSer = static_cast<SQLiteEntitySerializer*>(iter->second);
      sqlSer->useDatabase(NULL, NULL);
    }
  }
}
