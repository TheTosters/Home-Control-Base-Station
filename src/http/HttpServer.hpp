#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include <mutex>
#include "mongoose.h"

class HttpServer {
  public:
    HttpServer(int port = 8000);
  
    void start();
    void stop();
  private:
    int               httpPort;
    struct mg_mgr     manager;
    std::mutex        mutex;
    bool              stopLoop;
    bool              runLoopDone;
};

#endif