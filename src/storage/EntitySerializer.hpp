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
class Entity;

class EntitySerializer {
  public:
    virtual void store(Entity* data) = 0;
    virtual void storeOrUpdate(Entity* data) = 0;
};

#endif /* EntitySerializer_hpp */
