//
//  RestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "RestApiHandler.hpp"

void RestApiHandler::notImplemented(struct mg_connection* conn) {
  mg_printf(conn, "%s",
          "HTTP/1.0 501 Not Implemented\r\n"
          "Content-Length: 0\r\n\r\n");
}