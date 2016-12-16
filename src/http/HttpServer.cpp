#include "HttpServer.hpp"

static const int POOL_TIMEOUT = 1000;

void restApiHanlder(struct mg_connection *c, int event, void *data) {
  if (event == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) data;
    
  }
}

HttpServer::HttpServer(int port) : httpPort(port), stopLoop(false) {
  
}

void HttpServer::stop() {
  
  { //critical section
    std::unique_lock<std::mutex> lock(mutex);
    stopLoop = true;
    lock.unlock();
    
    //wait to server done
    for(;;) {
      lock.lock();
      if (runLoopDone == true) {
        lock.unlock();
        break;
      }
      lock.unlock();
    }
  }
  
  mg_mgr_free(&manager);
}

void HttpServer::start() {
  { //critical section
    std::unique_lock<std::mutex> lock(mutex);
    
    runLoopDone = false;
    stopLoop = false;
    
    struct mg_connection *c;
    
    mg_mgr_init(&manager, NULL);
    char address[255];
    sprintf(address, "%d", httpPort);
    c = mg_bind(&manager, address, restApiHanlder);
    mg_set_protocol_http_websocket(c);
    
    lock.unlock();
    
    for (;;) {
      mg_mgr_poll(&manager, POOL_TIMEOUT);
      
      lock.lock();
      if (stopLoop) {
        runLoopDone = true;
        lock.unlock();
        break;
      }
      
      lock.unlock();
    }
  }
}