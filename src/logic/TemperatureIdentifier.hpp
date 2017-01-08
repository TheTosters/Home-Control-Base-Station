//
//  TemperatureIdentifier.hpp
//  ci40-project5
//
//  Created by Bartłomiej on 09/12/16.
//  Copyright © 2016 Imagination Systems. All rights reserved.
//

#ifndef TemperatureIdentifier_hpp
#define TemperatureIdentifier_hpp

#include <stdio.h>
#include <string>

using namespace std;

class TemperatureIdentifier {
  public:
    TemperatureIdentifier(string name, double temp);
  
    string const&   getName() const;
    double          getTemperature();
  private:
    string    name;
    double    temperature;
};
#endif /* TemperatureIdentifier_hpp */
