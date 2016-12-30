//
//  FileRoomsSerializer.cpp
//  HomeControl
//
//  Created by Bartłomiej on 30/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <fstream>
#include "FileRoomsSerializer.hpp"
#include "JSONHelper.hpp"

const string FILE_NAME = "homePlan.json";

bool FileRoomsSerializer::store(shared_ptr<vector<shared_ptr<Room>>> data) {
  json jsonData = toJSON(data);
  
  ofstream outputFileStream(FILE_NAME);
  outputFileStream << jsonData.dump();
  bool result = !outputFileStream.bad();
  outputFileStream.close();
  
  return result;
}

shared_ptr<vector<shared_ptr<Room>>> FileRoomsSerializer::loadAll() {
  std::ifstream inputFileStream(FILE_NAME);
  if (inputFileStream.good() == false) {
    return make_shared<vector<shared_ptr<Room>>>();
  }
  
  std::stringstream buffer;
  buffer << inputFileStream.rdbuf();
  
  auto result = roomListFromJSON(buffer.str());
  
  return result;
}
