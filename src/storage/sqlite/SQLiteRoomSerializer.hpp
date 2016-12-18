//
//  SQLiteRoomSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 18/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SQLiteRoomSerializer_hpp
#define SQLiteRoomSerializer_hpp

#include <stdio.h>
#include "SQLiteEntitySerializer.hpp"

class  SQLiteRoomSerializer : protected SQLiteEntitySerializer {
  public:
    virtual void store(Entity* data) override;
    virtual void storeOrUpdate(Entity* data) override;
    virtual void useDatabase(sqlite3 *db, Storage* storage) override;
};
#endif /* SQLiteRoomSerializer_hpp */
