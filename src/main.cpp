//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "MasterBuilder.hpp"
#include "HttpServer.hpp"
#include "Logic.hpp"

int main(int argc, const char * argv[]) {

  MasterBuilder builder;  //use default file
  builder.build();
  shared_ptr<HttpServer> httpServer = builder.getHttpServer();
  shared_ptr<Logic> logic = builder.getLogic();
  
  
  logic->run();
  httpServer->start();
  return 0;
}
