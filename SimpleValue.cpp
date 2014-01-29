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

string Int::svalue() const
{
  stringstream ss(stringstream::out);
  ss << value;
  return ss.str();
}

string Float::svalue() const
{
  stringstream ss(stringstream::out);
  ss << setiosflags(ios::fixed) << setprecision(4) << value;
  return ss.str();
}

string Bool::svalue() const
{
  return value ? "true" : "false";
}

string Char::svalue() const
{
  return string(1, value);
}