//
//  RestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "RestApiHandler.hpp"

void RestApiHandler::methodNotSupported(struct mg_connection* conn, string const& supports) {
  string msg("Wrong method type, it must be one of listed here:");
  msg.append(supports);
  
  mg_printf(conn,
            "HTTP/1.0 405 Method Not Allowed\r\n"
            "Content-Length: %lu\r\n\r\n"
            "%s", msg.length(), msg.c_str());
  
}

void RestApiHandler::notImplemented(struct mg_connection* conn) {
  mg_printf(conn, "%s",
          "HTTP/1.0 501 Not Implemented\r\n"
          "Content-Length: 0\r\n\r\n");
}

void RestApiHandler::internalError(struct mg_connection* conn) {
  mg_printf(conn, "%s",
            "HTTP/1.0 500 Internal Server Error\r\n"
            "Content-Length: 0\r\n\r\n");
}

void RestApiHandler::sendHttpOk(struct mg_connection* conn) {
  mg_printf(conn, "%s",
            "HTTP/1.0 200 OK\r\n"
            "Content-Length: 0\r\n\r\n");
}

void RestApiHandler::missingQueryVariable(struct mg_connection* conn, string const& varName) {
  string msg("Expected query variable:");
  msg.append(varName);
  
  mg_printf(conn,
            "HTTP/1.0 400 Bad Request\r\n"
            "Content-Length: %lu\r\n\r\n"
            "%s", msg.length(), msg.c_str());
}

void RestApiHandler::bodyExpected(struct mg_connection* conn) {
  string msg("Expected body for this request.");
  
  mg_printf(conn,
            "HTTP/1.0 400 Bad Request\r\n"
            "Content-Length: %lu\r\n\r\n"
            "%s", msg.length(), msg.c_str());
}

void RestApiHandler::notFound(struct mg_connection* conn) {
  mg_printf(conn, "%s",
            "HTTP/1.0 404 Not Found\r\n"
            "Content-Length: 0\r\n\r\n");
}

void RestApiHandler::noContent(struct mg_connection* conn) {
  mg_printf(conn, "%s",
            "HTTP/1.0 204 No Content\r\n"
            "Content-Length: 0\r\n\r\n");
}

shared_ptr<string> RestApiHandler::getBodyOrDie(struct mg_connection* conn, void* rawData) {
  struct http_message *message = (struct http_message *) rawData;
  if (message->body.len == 0) {
    bodyExpected(conn);
    return nullptr;
  }
  return make_shared<string>(message->body.p, message->body.len);
}

bool RestApiHandler::getQueryVariable(void* rawData, string const& varName, long* result) {
  struct http_message *message = (struct http_message *) rawData;
  if (message->query_string.len == 0) {
    return false;
  }
  
  char idStr[25];
  if (mg_get_http_var(&message->query_string, varName.c_str(), idStr, sizeof(idStr)) < 0) {
    return false;
  }
  
  char* endptr;
  errno = 0;
  *result = strtol(idStr, &endptr, 0);
  if (errno == ERANGE || *endptr != '\0' || idStr == endptr) {
    return false;
  }
  
  return true;
}

bool RestApiHandler::getOrDieQueryVariable(struct mg_connection* conn, void* rawData, string const& varName, long* result) {
  bool queryResult = getQueryVariable(rawData, varName, result);

  if (queryResult == false) {
    missingQueryVariable(conn, varName);
    return false;
  }
  
  return true;
}