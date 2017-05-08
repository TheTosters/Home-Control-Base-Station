/*
 * SharedState.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: Zarnowski
 */

#include <logic/SharedState.h>

static const int zero = 0;

SharedState::SharedState() {
}

SharedState::~SharedState() {
}

const int& SharedState::operator[](const string& key) const {
  auto iter = states.find(key);
  return iter == states.end() ? zero : (*iter).second;
}

int& SharedState::operator[](const string& key) {
  return states[key];
}
