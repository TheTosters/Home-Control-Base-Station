//
//  StringHelper.hpp
//  HomeControl
//
//  Created by Bartłomiej on 06/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef StringHelper_hpp
#define StringHelper_hpp

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

using namespace std; //Don't if you're in a header-file

//taken from http://stackoverflow.com/a/26221725/2444937
template<typename ... Args>
string formatString( const string& format, Args ... args )
{
  size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
  unique_ptr<char[]> buf( new char[ size ] );
  snprintf( buf.get(), size, format.c_str(), args ... );
  return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

string trim(const string &s);
#endif /* StringHelper_hpp */
