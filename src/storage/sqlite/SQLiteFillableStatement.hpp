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
    bool bindNext(const double value);
    bool executeUpdate();
    long executeInsert();
  private:
    int             nextIndex;
    sqlite3_stmt*   statement;
    sqlite3*        db;
};
#endif /* SQLiteFillableStatement_hpp */
