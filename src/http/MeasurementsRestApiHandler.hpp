//
//  MeasurementsRestApiHandler.hpp
//  HomeControl
//
//  Created by Bartłomiej on 11/01/17.
//  Copyright © 2017 Imagination Systems. All rights reserved.
//

#ifndef MeasurementsRestApiHandler_hpp
#define MeasurementsRestApiHandler_hpp

#include <stdio.h>
#include <memory>
#include "RestApiHandler.hpp"
#include "Logic.hpp"

using namespace std;

class SimpleCriteria;

class MeasurementsRestApiHandler : public RestApiHandler {
  public:
    MeasurementsRestApiHandler(shared_ptr<Logic> logic);
    
    virtual void onGetRequest(struct mg_connection *c, void *data) override;
    virtual void onPostRequest(struct mg_connection *c, void *data) override;
    virtual void onDeleteRequest(struct mg_connection *c, void *data) override;
  private:
    shared_ptr<Logic> logic;
  
    void parseCriteria(void *data, SimpleCriteria& criteria, bool parseNavi);
};

#endif /* MeasurementsRestApiHandler_hpp */
