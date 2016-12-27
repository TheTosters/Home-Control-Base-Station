//
//  HomePlanRestApiHandler.cpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#include "HomePlanRestApiHandler.hpp"

HomePlanRestApiHandler::HomePlanRestApiHandler() : RestApiHandler("/plan") {
  
}

void HomePlanRestApiHandler::onGetRequest(struct mg_connection *c, void *data) {
  printf("GET\n");
}

void HomePlanRestApiHandler::onPostRequest(struct mg_connection *c, void *data) {
  
}

void HomePlanRestApiHandler::onDeleteRequest(struct mg_connection *c, void *data) {
  
}
