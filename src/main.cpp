//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <iostream>
#include "mongoose.h"
#include "HttpServer.hpp"

HttpServer* httpServer;

int main(int argc, const char * argv[]) {
  httpServer = new HttpServer();
  httpServer->start();
  return 0;
}
