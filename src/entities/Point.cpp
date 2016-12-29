//
//  Point.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Point.hpp"

#define SERIALIZER_POINT_ID  4

Point::Point()
: Entity(SERIALIZER_POINT_ID), x(0), y(0) {
  
}

Point::Point(int _x, int _y)
: Entity(SERIALIZER_POINT_ID), x(_x), y(_y), ownerId(-1) {

}

Point::Point(int _id, int _x, int _y)
: Entity(_id, SERIALIZER_POINT_ID), x(_x), y(_y), ownerId(-1) {
  
}

Point::Point(Point* source)
:Entity(source != NULL ? source->id : -1, SERIALIZER_POINT_ID),
 x(source != NULL ? source->x : 0),
 y(source != NULL ? source->y : 0),
 ownerId(source != NULL ? source->ownerId : -1){

}

int Point::getX() {
  return x;
}

int Point::getY() {
  return y;
}

void Point::setX(int x) {
  this->x = x;
}

void Point::setY(int y) {
  this->y = y;
}

long Point::getOwnerId() {
  return ownerId;
}

void Point::setOwnerId(long ownerId) {
  this->ownerId = ownerId;
}
