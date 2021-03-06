#include <thread>
#include "http/HttpServer.hpp"
#include "misc/LogHelper.hpp"

static const int POOL_TIMEOUT = 1000;
static const struct mg_str METHOD_GET = MG_MK_STR("GET");
static const struct mg_str METHOD_POST = MG_MK_STR("POST");
static const struct mg_str METHOD_DELETE = MG_MK_STR("DELETE");

//static int hasPrefix(const struct mg_str *uri, const struct mg_str *prefix) {
//  return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
//}

static int isEqual(const struct mg_str *s1, const struct mg_str *s2) {
  return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

void restApiHanlder(struct mg_connection *c, int event, void *data) {
  if (event == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) data;
    HttpServer* server = static_cast<HttpServer*>(c->mgr->user_data);
    
    if (isEqual(&hm->method, &METHOD_GET)) {
      server->onGetRequest(c, data);
      
    } else if (isEqual(&hm->method, &METHOD_POST)) {
      server->onPostRequest(c, data);
      
    } else if (isEqual(&hm->method, &METHOD_DELETE)) {
      server->onDeleteRequest(c, data);
      
    } else {
      mg_printf(c, "%s",
                "HTTP/1.0 501 Not Implemented\r\n"
                "Content-Length: 0\r\n\r\n");
    }
    
  }
}

HttpServer::HttpServer(shared_ptr<Storage> stor, int port)
: httpPort(port), stopLoop(false), runLoopDone(false), storage(stor) {
  
}

shared_ptr<Storage> HttpServer::getStorage() {
  return storage;
}

void HttpServer::stop(bool waitForStop) {
  
  { //critical section
    std::unique_lock<std::mutex> lock(mutex);
    stopLoop = true;
    lock.unlock();
    
    //wait to server done
    for(;waitForStop;) {
      lock.lock();
      if (runLoopDone == true) {
        lock.unlock();
        break;
      }
      lock.unlock();
      this_thread::sleep_for(chrono::milliseconds(50));
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
    manager.user_data = this;
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
  runLoopDone = true;
}

void HttpServer::onGetRequest(struct mg_connection *c, void *data) {
  struct http_message *hm = (struct http_message *) data;
  string uriStr(hm->uri.p, hm->uri.len);
  shared_ptr<RestApiHandler> handler = handlers[uriStr];
  
  if (handler) {
    handler->onGetRequest(c, data);
    
  } else {
    spdlog::get(WEB_LOGGER_NAME)->warn("No handler for request:{}", uriStr);
  }
}

void HttpServer::onPostRequest(struct mg_connection *c, void *data) {
  struct http_message *hm = (struct http_message *) data;
  string uriStr(hm->uri.p, hm->uri.len);
  shared_ptr<RestApiHandler> handler = handlers[uriStr];
  
  if (handler) {
    handler->onPostRequest(c, data);
    
  } else {
    spdlog::get(WEB_LOGGER_NAME)->warn("No handler for request:{}", uriStr);
  }
}

void HttpServer::onDeleteRequest(struct mg_connection *c, void *data) {
  struct http_message *hm = (struct http_message *) data;
  string uriStr(hm->uri.p, hm->uri.len);
  shared_ptr<RestApiHandler> handler = handlers[uriStr];
  
  if (handler) {
    handler->onDeleteRequest(c, data);
    
  } else {
    spdlog::get(WEB_LOGGER_NAME)->warn("No handler for request:{}", uriStr);
  }
}

void HttpServer::registerHandler(shared_ptr<RestApiHandler> const& handler) {
  if (handlers.find( handler->getEndpoint() ) != handlers.end()) {
    spdlog::get(MISC_LOGGER_NAME)->error("  Endpoint {} already registered!", handler->getEndpoint());
    return;
  }
  handlers[handler->getEndpoint()] = handler;
  handler->setServer(this);
}
