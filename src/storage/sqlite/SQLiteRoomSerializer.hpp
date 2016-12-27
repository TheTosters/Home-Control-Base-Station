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
#include <vector>

#include "SQLiteEntitySerializer.hpp"
#include "Room.hpp"
#include "SimpleCriteria.hpp"

class  SQLiteRoomSerializer : public SQLiteEntitySerializer {
  public:
    void useDatabase(sqlite3 *db, Storage* storage);
  
    void store(Entity* data);
    void storeOrUpdate(Entity* data);
  
    shared_ptr<vector<shared_ptr<Room>>> loadAll();
    shared_ptr<vector<shared_ptr<Room>>> loadMatching(SimpleCriteria criteria);
    shared_ptr<Room> load(long id);
};
#endif /* SQLiteRoomSerializer_hpp */
