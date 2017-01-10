//
//  PhysicalSensorRestApiHandler.hpp
//  HomeControl
//
//  Created by Bartłomiej on 10/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef PhysicalSensorRestApiHandler_hpp
#define PhysicalSensorRestApiHandler_hpp

#include <stdio.h>
#include "RestApiHandler.hpp"
#include "Logic.hpp"

using namespace std;

class PhysicalSensorRestApiHandler : public RestApiHandler {
  public:
    PhysicalSensorRestApiHandler(shared_ptr<Logic> logic);
    
    virtual void onGetRequest(struct mg_connection *c, void *data) override;
    virtual void onPostRequest(struct mg_connection *c, void *data) override;
    virtual void onDeleteRequest(struct mg_connection *c, void *data) override;
  private:
    shared_ptr<Logic> logic;
};

#endif /* PhysicalSensorRestApiHandler_hpp */
