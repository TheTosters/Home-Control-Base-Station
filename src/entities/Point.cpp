//
//  Point.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "Point.hpp"

#define SERIALIZER_POINT_ID  4

Point::Point(int _x, int _y)
: Entity(SERIALIZER_POINT_ID), x(_x), y(_y) {

}

Point::Point(int _id, int _x, int _y)
: Entity(_id, SERIALIZER_POINT_ID), x(_x), y(_y) {
  
}

Point::Point(Point* source)
:Entity(source != NULL ? source->id : 0, SERIALIZER_POINT_ID),
 x(source != NULL ? source->x : 0),
 y(source != NULL ? source->y : 0) {

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
