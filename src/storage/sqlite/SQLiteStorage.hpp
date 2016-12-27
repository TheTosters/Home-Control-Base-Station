//
//  SQLiteStorage.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteStorage_hpp
#define SQLiteStorage_hpp

#include <stdio.h>
#include <sqlite3.h>
#include "Storage.hpp"

class SQLiteStorage : public Storage {
  public:
    SQLiteStorage() : db(NULL) {};
    virtual bool open() override;
    virtual void close() override;

  protected:
    sqlite3*  db;
};

#endif /* SQLiteStorage_hpp */
