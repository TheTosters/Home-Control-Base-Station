//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <iostream>
#include "Room.hpp"
#include "Sensor.hpp"

int main(int argc, const char * argv[]) {
  std::auto_ptr<Room> r(new Room(1, "dsf"));
  vector<Point>* shape = r->getShape();
  
  shape->push_back( Point(1, 2, 3) );
  
  Point pp = r->getShape()->at(0);
  
  return 0;
}
