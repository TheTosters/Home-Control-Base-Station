//
//  SQLiteEntitySerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "storage/sqlite/SQLiteEntitySerializer.hpp"
#include "storage/Storage.hpp"

void SQLiteEntitySerializer::useDatabase(sqlite3 *db, Storage* storage) {
  this->db = db;
  this->storage = storage;
}

bool SQLiteEntitySerializer::executeUpdateQuery(string creationSql) {
  
  char* zErrMsg = NULL;
  int rc = sqlite3_exec(db, creationSql.c_str(), NULL, 0, &zErrMsg);
  
  if( rc != SQLITE_OK ){
    //TODO: logs
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  return rc == SQLITE_OK;
}
