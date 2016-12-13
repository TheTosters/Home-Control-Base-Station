//
//  Entity.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

class Entity {
  public:
    Entity(int _id, int _serializerId) : id(_id), serializerId(_serializerId) {};
    Entity(int _serializerId) : id(-1), serializerId(_serializerId) {};
  
    int getId() {return id;}
    void setId(int _id) {id = _id;}
    int getSerializerId() {return serializerId;}
  protected:
    int   id;
    int   serializerId;
};

#endif /* Entity_h */
