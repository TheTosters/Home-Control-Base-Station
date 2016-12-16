//
//  HomePlanRestApiHandler.hpp
//  HomeControl
//
//  Created by Bartłomiej on 16/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef HomePlanRestApiHandler_hpp
#define HomePlanRestApiHandler_hpp

#include <stdio.h>
#include "RestApiHandler.hpp"

class HomePlanRestApiHandler : protected RestApiHandler {
  public:
    HomePlanRestApiHandler();
    virtual void onGetRequest(struct mg_connection *c, void *data) override;
    virtual void onPostRequest(struct mg_connection *c, void *data) override;
    virtual void onDeleteRequest(struct mg_connection *c, void *data) override;
  
};
#endif /* HomePlanRestApiHandler_hpp */
