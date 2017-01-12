//
//  SimpleCriteria.hpp
//  HomeControl
//
//  Created by Bartłomiej on 19/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef SimpleCriteria_hpp
#define SimpleCriteria_hpp

#include <stdio.h>
#include <time.h>
#include <vector>
#include <memory>

using namespace std;

class SimpleCriteria {
public:
  shared_ptr<string>  name;
  long                id;
  long                helperId; //additional id, Serializer dependent
  long                type; //misc value, Serializer dependent
  time_t              from;
  time_t              to;
  long                offset; //for pagination
  long                count;  //for pagination
  
  SimpleCriteria() : id(-1), helperId(-1), type(-1), from(0), to(0), offset(0), count(0) {}
};

#endif /* SimpleCriteria_hpp */
