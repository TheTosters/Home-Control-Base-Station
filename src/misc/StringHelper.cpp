//
//  StringHelper.cpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#include "StringHelper.hpp"

string trim(const string &s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && isspace(*it))
    it++;
  
  std::string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && isspace(*rit))
    rit++;
  
  return std::string(it, rit.base());
}