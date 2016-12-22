//
//  EntitySerializer.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef EntitySerializer_hpp
#define EntitySerializer_hpp

#include <stdio.h>
#include <vector>
#include "SimpleCriteria.hpp"
#include "Entity.hpp"

using namespace std;

class EntitySerializer {
  public:
    virtual void store(Entity* data) = 0;
    virtual void storeOrUpdate(Entity* data) = 0;
    virtual void loadAll(shared_ptr<vector<shared_ptr<Entity>>> result) = 0;
    virtual void loadMatching(SimpleCriteria criteria, shared_ptr<vector<shared_ptr<Entity>>> result) = 0;
    virtual shared_ptr<Entity> load(long id) = 0;
};

#endif /* EntitySerializer_hpp */
