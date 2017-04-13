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
#include <memory>
#include "http/RestApiHandler.hpp"
#include "logic/Logic.hpp"

using namespace std;

class PhysicalSensorRestApiHandler : public RestApiHandler {
  public:
    PhysicalSensorRestApiHandler(shared_ptr<Logic> logic);
    virtual ~PhysicalSensorRestApiHandler() = default;
    
    virtual void onGetRequest(struct mg_connection *c, void *data) override;
    virtual void onPostRequest(struct mg_connection *c, void *data) override;
    virtual void onDeleteRequest(struct mg_connection *c, void *data) override;
  private:
    shared_ptr<Logic> logic;
    void handleScanRequest(struct mg_connection *c);
    void handleLastScan(struct mg_connection *c);
    void handleMeasurements(struct mg_connection *c, void *data);
    void handleAddScanned(struct mg_connection *c, void *data);
};

#endif /* PhysicalSensorRestApiHandler_hpp */
