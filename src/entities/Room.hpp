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
#include "Sensor.hpp"

using namespace std;

class Room : public Entity {
  public:
    Room();
    Room(string name);
    Room(int id, string name);
  
    string getName();
    void   setName(string name);
    int    getFloor();
    void   setFloor(int floor);
    vector<Point>* getShape();
    vector<Sensor>* getSensors();
  private:
    string            name;
    int               floor;
    vector<Sensor>    sensors;
    vector<Point>     shape;
};

typedef vector<shared_ptr<Room>> RoomsVector;
typedef shared_ptr<RoomsVector> RoomsList;

#endif /* Room_hpp */
