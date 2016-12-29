//
//  SQLiteFillableStatement.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteFillableStatement_hpp
#define SQLiteFillableStatement_hpp

#include <stdio.h>
#include <string>

#include "sqlite3.h"

using namespace std;

class SQLiteFillableStatement {
  public:
    SQLiteFillableStatement(sqlite3* db, const string sql);
    ~SQLiteFillableStatement();
  
    bool bindNext(const std::string& value);
    bool bindNext(const int value);
    bool bindNext(const long value);
    bool bindNext(const double value);
    bool executeUpdate(int* count = nullptr);
    long executeInsert();
    // <0 error, 0 - no more data, >0 row loaded
    int executeSelectNext();
    template <typename T> void getColumns(T* t) {
      getSingleColumn(t);
    }
  
    template<typename T, typename... Args> void getColumns(T* t, Args... args) {
      getSingleColumn(t);
      getColumns(args...);
    }
  private:
    int             nextBindIndex;
    int             nextColumnIndex;
    sqlite3_stmt*   statement;
    sqlite3*        db;

    void getSingleColumn(int* result);
    void getSingleColumn(long* result);
    void getSingleColumn(double* result);
    void getSingleColumn(std::string* result);
};

#endif /* SQLiteFillableStatement_hpp */
