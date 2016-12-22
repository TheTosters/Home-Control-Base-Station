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

void Storage::registerSereializer(Entity* entityType, EntitySerializer* serializer) {
  map<int, EntitySerializer*>::iterator it = serializers.find(entityType->getSerializerId());
  if(it == serializers.end()) {
    serializers[entityType->getSerializerId()] = serializer;
  }
}

void Storage::add(Entity* data) {
  EntitySerializer* serializer = serializers[data->getSerializerId()];
  serializer->store(data);
}

void Storage::addOrUpdate(Entity* data) {
  EntitySerializer* serializer = serializers[data->getSerializerId()];
  serializer->storeOrUpdate(data);
}

template<class T> shared_ptr<vector<shared_ptr<T>>> Storage::loadAll() {
  auto result = make_shared<vector<shared_ptr<T>>>();
  EntitySerializer* serializer = serializers[T().getSerializerId()];
  serializer->loadAll(result);
  return result;
}

template<class T> shared_ptr<vector<shared_ptr<T>>> Storage::loadMatching(SimpleCriteria criteria) {
  auto result = make_shared<vector<shared_ptr<T>>>();
  EntitySerializer* serializer = serializers[T().getSerializerId()];
  serializer->loadMatching(result, criteria);
  return result;
}

template<class T> shared_ptr<T> Storage::load(long id) {
  if (id < 0) {
    //todo: logging
    printf("Id expected here!\n");
    return nullptr;
  }
  EntitySerializer* serializer = serializers[T().getSerializerId()];
  return serializer->load(id);
}
