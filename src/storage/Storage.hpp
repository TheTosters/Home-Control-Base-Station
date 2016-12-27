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
#include "EntitySerializer.hpp"

using namespace std;

class Storage {
  public:
    virtual bool open() = 0;
    virtual void close() = 0;
    
    virtual void registerSereializer(Entity* entityType, EntitySerializer* serializer);
    template<class T> T* requestSerializer(Entity marker) {
      return static_cast<T*>(serializers[marker.getSerializerId()]);
    }
  protected:
    map<int, EntitySerializer*>  serializers;
};

#endif /* Storage_hpp */
