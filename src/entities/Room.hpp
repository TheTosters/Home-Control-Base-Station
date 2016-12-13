//
//  Room.hpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef Room_hpp
#define Room_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Point.hpp"
#include "Entity.hpp"

using namespace std;

class Sensor;

class Room : Entity {
  public:
    Room(int id, string name);
    Room(string name);
  
    string getName();
    void   setName(string name);
    vector<Point>* getShape();
  private:
    string            name;
    vector<Sensor>    sensors;
    vector<Point>     shape;
};

#endif /* Room_hpp */
