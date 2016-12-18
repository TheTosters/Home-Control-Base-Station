//
//  SQLiteFillableStatement.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "SQLiteFillableStatement.hpp"

SQLiteFillableStatement::SQLiteFillableStatement(sqlite3* _db, const string sql) : db(_db), nextIndex(1), statement(NULL) {

  int res = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL);
  if (res != SQLITE_OK) {
    //todo: logs
    fprintf(stderr, "SQL error, can't create statement: %s", sql.c_str());
  }
}

SQLiteFillableStatement::~SQLiteFillableStatement() {
  sqlite3_finalize(statement);
  db = NULL;
  statement = NULL;
}

bool SQLiteFillableStatement::bindNext(const long value) {
  int res = sqlite3_bind_int64(statement, nextIndex, value);
  nextIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const std::string& value) {
  int res = sqlite3_bind_text(statement, nextIndex, value.c_str(), -1, SQLITE_TRANSIENT);
  nextIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const int value) {
  int res = sqlite3_bind_int(statement, nextIndex, value);
  nextIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const double value) {
  int res = sqlite3_bind_double(statement, nextIndex, value);
  nextIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::executeUpdate() {
  int rc = sqlite3_step(statement);
  if(SQLITE_DONE != rc) {
    //todo: logs
    fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));
  }
  return rc == SQLITE_DONE;
}

long SQLiteFillableStatement::executeInsert() {
  executeUpdate();
  return sqlite3_last_insert_rowid(db);
}