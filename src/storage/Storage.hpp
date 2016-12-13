//
//  Storage.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <stdio.h>
#include <typeinfo>
#include <map>

using namespace std;

class Entity;
class EntitySerializer;

class Storage {
  public:
    virtual bool open() = 0;
    virtual void close() = 0;
    
    virtual void registerSereializer(Entity* entityType, EntitySerializer* serializer);
    
    virtual void add(Entity* data) = 0;
    virtual void addOrUpdate(Entity* data) = 0;
  protected:
    map<int, EntitySerializer*>  serializers;
};

#endif /* Storage_hpp */
