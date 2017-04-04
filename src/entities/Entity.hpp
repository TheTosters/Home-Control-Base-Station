//
//  Entity.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Entity_h
#define Entity_h
#include <memory>

class Entity {
  public:
    Entity(long _id, int _serializerId) : id(_id), serializerId(_serializerId) {};
    Entity(int _serializerId) : id(-1), serializerId(_serializerId) {};
  
    long getId() {return id;}
    void setId(long _id) {id = _id;}
    int getSerializerId() {return serializerId;}
  protected:
    long  id;
    int   serializerId;
};

#endif /* Entity_h */
