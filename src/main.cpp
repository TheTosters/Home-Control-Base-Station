//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <string>
#include "MasterBuilder.hpp"
#include "HttpServer.hpp"
#include "Logic.hpp"
#include <TCLAP/CmdLine.h>

using namespace TCLAP;
using namespace std;

int main(int argc, const char * argv[]) {

  shared_ptr<MasterBuilder> builder;
  
  try {
    CmdLine cmd("Home Control Gateway Application", ' ', "0.0.1");
    ValueArg<string> configFile("c", "config", "Path to config file", false, DEFAULT_CONFIG_FILE, "string");
    cmd.add( configFile );
    
    ValueArg<int> logLevel("v", "logLevel", "Logging level 0-DEBUG, 5-ERROR", false, 4, "number");
    cmd.add( logLevel );
    
    cmd.parse( argc, argv );
    
    builder = make_shared<MasterBuilder>(configFile.getValue());
  } catch (ArgException &e) {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return -1;
  }
  
  builder->build();
  shared_ptr<HttpServer> httpServer = builder->getHttpServer();
  shared_ptr<Logic> logic = builder->getLogic();
  
  logic->run();
  httpServer->start();
  return 0;
}
