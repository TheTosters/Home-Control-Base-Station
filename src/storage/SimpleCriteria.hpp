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

using namespace std;

class SimpleCriteria {
public:
  shared_ptr<string>  name;
  long                id;
  long                helperId; //additional id, Serializer dependent
  time_t              from;
  time_t              to;
};

#endif /* SimpleCriteria_hpp */
