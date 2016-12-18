#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <mutex>
#include <map>
#include "mongoose.h"
#include "RestApiHandler.hpp"

class HttpServer {
  public:
    HttpServer(int port = 8000);
  
    void start();
    void stop();
  
    void onGetRequest(struct mg_connection *c, void *data);
    void onPostRequest(struct mg_connection *c, void *data);
    void onDeleteRequest(struct mg_connection *c, void *data);

    void registerHandler(shared_ptr<RestApiHandler> const& handler);
  private:
    int                     httpPort;
    struct mg_mgr           manager;
    std::mutex              mutex;
    bool                    stopLoop;
    bool                    runLoopDone;
    map<string, shared_ptr<RestApiHandler>> handlers;
};

#endif