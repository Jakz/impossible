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


void Lambda::put(size_t index, Value value)
{
  index = std::min(index, _code->size());
}

integral_t Lambda::size() const { return _code->size(); }
