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

std::string LazyArray::svalue() const
{
  std::string s("(? ");
  s += data.code()->svalue();
  s += " )";
  return s;
}

std::string Lambda::svalue() const
{
  std::stringstream ss(std::stringstream::out);
  
  ss << "[";
  
  for (int i = 0; i < _code->len(); ++i)
  {
    ss << _code->at(i)->svalue();
    if (i < _code->len() - 1)
      ss << " ";
  }
  
  ss << "]";
  
  return ss.str();
}
