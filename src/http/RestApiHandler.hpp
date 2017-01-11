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
  
    string getEndpoint() const {return endpoint; }

  protected:
    HttpServer* server;
    string      endpoint;
  
    //those methods returns various errors to connection
    void  notImplemented(struct mg_connection* conn);
    void  methodNotSupported(struct mg_connection* conn, string const& supports);
    void  missingQueryVariable(struct mg_connection* conn, string const& varName);
    void  notFound(struct mg_connection* conn);
    void  bodyExpected(struct mg_connection* conn);
    void  internalError(struct mg_connection* conn);
    void  noContent(struct mg_connection* conn);
    void  badRequest(struct mg_connection* conn);
    void  conflict(struct mg_connection* conn);
  
    void  sendHttpOk(struct mg_connection* conn);
  
    //helper methods
    bool getQueryVariable(void* rawData, string const& varName, long* result);
    bool getOrDieQueryVariable(struct mg_connection* conn, void* rawData, string const& varName, long* result);
    shared_ptr<string> getBodyOrDie(struct mg_connection* conn, void* rawData);
};
#endif /* RestApiHandler_hpp */
