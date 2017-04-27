/*
 * SharedState.h
 *
 *  Created on: Apr 27, 2017
 *      Author: Zarnowski
 */

#ifndef SharedState_hpp
#define SharedState_hpp

#include <unordered_map>
#include <string>

using namespace std;

class SharedState {
  public:
    SharedState();
    virtual ~SharedState();
    const int& operator[](const string& key) const;
    int& operator[](const string& key);
  private:
    unordered_map<string, int> states;
};

#endif /* SharedState_hpp */
