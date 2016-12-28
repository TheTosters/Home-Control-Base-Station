//
//  RoomsRestApiHandler.hpp
//  HomeControl
//
//  Created by Bartłomiej on 28/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef RoomsRestApiHandler_hpp
#define RoomsRestApiHandler_hpp

#include <stdio.h>
#include "RestApiHandler.hpp"

class RoomsRestApiHandler : public RestApiHandler {
public:
  RoomsRestApiHandler();
  
  virtual void onGetRequest(struct mg_connection *c, void *data) override;
  virtual void onPostRequest(struct mg_connection *c, void *data) override;
  virtual void onDeleteRequest(struct mg_connection *c, void *data) override;
  
};

#endif /* RoomsRestApiHandler_hpp */
