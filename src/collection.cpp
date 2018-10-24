//
//  CompositeTypes.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#include "collection.h"
#include "traits.h"
#include "instruction.h"

#include <iostream>
#include <iomanip>
#include <sstream>

const Value& String::next() const
{
  if (it.it == value.end())
    return Value::INVALID;
  else
  {
    it.value = *it.it++;
    return it.value;
  }
}
