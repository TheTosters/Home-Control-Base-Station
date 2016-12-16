//
//  RestApiHandler.hpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef RestApiHandler_hpp
#define RestApiHandler_hpp

#include <stdio.h>
#include <string>
#include "mongoose.h"

using namespace std;
class HttpServer;

class RestApiHandler {
  public:
    RestApiHandler(string _endpoint) : endpoint(_endpoint) {}
  
    virtual void onGetRequest(struct mg_connection *c, void *data) {}
    virtual void onPostRequest(struct mg_connection *c, void *data) {}
    virtual void onDeleteRequest(struct mg_connection *c, void *data) {}
  
    void  setServer(HttpServer* serv) { server = serv;}
    HttpServer* getServer() { return server;}
  
    string getEndpoint() {return endpoint; }
  protected:
    HttpServer* server;
    string      endpoint;
};
#endif /* RestApiHandler_hpp */
