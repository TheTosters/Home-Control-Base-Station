//
//  SQLiteFillableStatement.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "storage/sqlite/SQLiteFillableStatement.hpp"

SQLiteFillableStatement::SQLiteFillableStatement(sqlite3* _db, const string sql) : nextBindIndex(1), nextColumnIndex(0), statement(NULL), db(_db) {

  int res = sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, NULL);
  if (res != SQLITE_OK) {
    //todo: logs
    fprintf(stderr, "SQL error, can't create statement: %s [%s]", sql.c_str(), sqlite3_errmsg(db));
  }
}

SQLiteFillableStatement::~SQLiteFillableStatement() {
  sqlite3_finalize(statement);
  db = NULL;
  statement = NULL;
}

bool SQLiteFillableStatement::bindNext(const long value) {
  int res = sqlite3_bind_int64(statement, nextBindIndex, value);
  nextBindIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const std::string& value) {
  int res = sqlite3_bind_text(statement, nextBindIndex, value.c_str(), -1, SQLITE_TRANSIENT);
  nextBindIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const int value) {
  int res = sqlite3_bind_int(statement, nextBindIndex, value);
  nextBindIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::bindNext(const double value) {
  int res = sqlite3_bind_double(statement, nextBindIndex, value);
  nextBindIndex ++;
  return res ==  SQLITE_OK;
}

bool SQLiteFillableStatement::executeUpdate(int* count) {
  int rc = sqlite3_step(statement);
  if(SQLITE_DONE != rc) {
    //todo: logs
    fprintf(stderr, "update statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));
  }
  if (count != nullptr) {
    *count = sqlite3_changes(db);
  }
  return rc == SQLITE_DONE;
}

long SQLiteFillableStatement::executeInsert() {
  executeUpdate();
  return sqlite3_last_insert_rowid(db);
}

int SQLiteFillableStatement::executeSelectNext() {
  int rc = sqlite3_step(statement);
  nextColumnIndex = 0;
  
  switch(rc) {
    case SQLITE_DONE:
      fprintf(stdout, "End of select data\n");
      return 0;
      
    case SQLITE_ROW:
      return 1;
      
    default:
      fprintf(stderr, "Select statement returned error (%i): %s\n", rc, sqlite3_errmsg(db));
      return -1;
  }
}

void SQLiteFillableStatement::getSingleColumn(int* result) {
  if (nextColumnIndex >= sqlite3_column_count(statement)) {
    fprintf(stderr, "[Int]Column out of index %d (max:%d) in statement\n", nextColumnIndex, sqlite3_column_count(statement));
    return;
  }
  *result = sqlite3_column_int(statement, nextColumnIndex);
  nextColumnIndex++;
}

void SQLiteFillableStatement::getSingleColumn(long* result) {
  if (nextColumnIndex >= sqlite3_column_count(statement)) {
    fprintf(stderr, "[Long]Column out of index %d (max:%d) in statement\n", nextColumnIndex, sqlite3_column_count(statement));
    return;
  }
  *result = sqlite3_column_int64(statement, nextColumnIndex);
  nextColumnIndex++;
}

void SQLiteFillableStatement::getSingleColumn(double* result) {
  if (nextColumnIndex >= sqlite3_column_count(statement)) {
    fprintf(stderr, "[Double]Column out of index %d (max:%d) in statement\n", nextColumnIndex, sqlite3_column_count(statement));
    return;
  }
  *result = sqlite3_column_double(statement, nextColumnIndex);
  nextColumnIndex++;
}

void SQLiteFillableStatement::getSingleColumn(std::string* result) {
  if (nextColumnIndex >= sqlite3_column_count(statement)) {
    fprintf(stderr, "[String]Column out of index %d (max:%d) in statement\n", nextColumnIndex, sqlite3_column_count(statement));
    return;
  }
  const char* data = reinterpret_cast<const char*>(sqlite3_column_text(statement, nextColumnIndex));
  result->append(data);
  nextColumnIndex++;
}
