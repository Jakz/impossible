//
//  integral_types.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#include "SimpleValue.h"

#include <iostream>
#include <iomanip>
#include <sstream>

string Bool::svalue() const
{
  return value ? "true" : "false";
}

string Char::svalue() const
{
  return string(1, value);
}
