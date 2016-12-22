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
#include <map>
#include <vector>
#include "Entity.hpp"
#include "SimpleCriteria.hpp"

using namespace std;

class EntitySerializer;

class Storage {
  public:
    virtual bool open() = 0;
    virtual void close() = 0;
    
    virtual void registerSereializer(Entity* entityType, EntitySerializer* serializer);
    
    virtual void add(Entity* data) = 0;
    virtual void addOrUpdate(Entity* data) = 0;
  
    template<class T> shared_ptr<vector<shared_ptr<T>>> loadAll();
    template<class T> shared_ptr<vector<shared_ptr<T>>> loadMatching(SimpleCriteria criteria);
    template<class T> shared_ptr<T> load(long id);
  protected:
    map<int, EntitySerializer*>  serializers;
};

#endif /* Storage_hpp */
