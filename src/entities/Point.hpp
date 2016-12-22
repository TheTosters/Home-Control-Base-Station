//
//  Point.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>
#include "Entity.hpp"

class Point : public Entity {
  public:
    Point(Point* source);
    Point(int id, int x, int y);
    Point(int x, int y);
  
    int     getX();
    int     getY();
    void    setX(int x);
    void    setY(int y);
  private:
    int   x, y;
};

#endif /* Point_hpp */
