//
//  main.cpp
//  HomeControl
//
//  Created by Bartłomiej on 08/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include <string>
#include <tclap/CmdLine.h>
#include <signal.h>
#include "MasterBuilder.hpp"
#include "http/HttpServer.hpp"
#include "logic/Logic.hpp"
#include "misc/LogHelper.hpp"

using namespace TCLAP;
using namespace std;

struct Components {
  shared_ptr<HttpServer> httpServer;
  shared_ptr<Logic> logic;
};

static struct Components components = {
  .httpServer = NULL,
  .logic = NULL
};

void signalHandler(int sig, siginfo_t *siginfo, void *context) {
  cout<<"Signal to terminate received!" << endl;
  components.logic->terminate();
  components.httpServer->stop();
}

bool attachSignalsHandlers() {
  struct sigaction act;
  
  memset (&act, '\0', sizeof(act));
  act.sa_sigaction = &signalHandler;
  act.sa_flags = SA_SIGINFO;
  
  if (sigaction(SIGTERM, &act, NULL) < 0) {
    cerr << "error: " << "Can't attach to signal handlers" << endl;
    return false;
  }

  if (sigaction(SIGINT, &act, NULL) < 0) {
    cerr << "error: " << "Can't attach to signal handlers" << endl;
    return false;
  }

  return true;
}

int main(int argc, const char * argv[]) {

  if (attachSignalsHandlers() == false) {
    return -1;
  }
  
  shared_ptr<MasterBuilder> builder;
  
  try {
    CmdLine cmd("Home Control Gateway Application", ' ', "0.0.1");
    ValueArg<string> configFile("c", "config", "Path to config file", false, DEFAULT_CONFIG_FILE, "path");
    cmd.add( configFile );
    
    ValueArg<int> logLevel("v", "logLevel", "Logging level (trace = 0, debug = 1, info = 2, warn = 3, err = 4, critical = 5, off = 6)", false, 3, "number");
    cmd.add( logLevel );
    
    cmd.parse( argc, argv );
    
    builder = make_shared<MasterBuilder>(configFile.getValue());

    //this must be after builder
    if (logLevel.isSet()) {
      overrideLoggersLevel(logLevel.getValue());
    }
    
  } catch (ArgException &e) {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return -1;
  }
  
  builder->build();
  components.httpServer = builder->getHttpServer();
  components.logic = builder->getLogic();
  
  components.logic->run();
  components.httpServer->start();
  
  return 0;
}
