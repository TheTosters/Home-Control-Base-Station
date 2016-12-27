//
//  Storage.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Storage.hpp"
#include "Entity.hpp"
#include "EntitySerializer.hpp"

void Storage::registerSerializer(Entity entityType, EntitySerializer* serializer) {
  map<int, EntitySerializer*>::iterator it = serializers.find(entityType.getSerializerId());
  if(it == serializers.end()) {
    serializers[entityType.getSerializerId()] = serializer;
  }
}
