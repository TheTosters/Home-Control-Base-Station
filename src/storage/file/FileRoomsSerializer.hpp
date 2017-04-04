//
//  FileRoomsSerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef FileRoomsSerializer_hpp
#define FileRoomsSerializer_hpp

#include <stdio.h>
#include <vector>
#include "entities/Room.hpp"
#include "storage/EntitySerializer.hpp"

class FileRoomsSerializer : public EntitySerializer {
  public:
    bool store(shared_ptr<vector<shared_ptr<Room>>> data);
    
    shared_ptr<vector<shared_ptr<Room>>> loadAll();
};

#endif /* FileRoomsSerializer_hpp */
