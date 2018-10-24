//
//  CompositeTypes.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#include "CompositeValue.h"
#include "TypeTraits.h"
#include "instruction.h"

#include <iostream>
#include <iomanip>
#include <sstream>

const Value& String::next() const
{
  if (it.it == value.end())
    return Value(TYPE_INVALID);
  else
  {
    it.value = *it.it++;
    return it.value;
  }
}

template<typename T>
struct CollectionPrinter
{
  std::string prefix;
  std::string suffix;
  std::string separator;
  std::function<std::string(const T&)> valuePrinter;
  
  std::string svalue(const TCollection* collection) const
  {
    std::stringstream ss(std::stringstream::out);
    bool first = true;
    collection->iterate();
    
    ss << prefix;
    
    while (collection->hasNext())
    {
      auto it = collection->next();
      
      if (first) first = false;
      else ss << separator;
      
      ss << valuePrinter(it);
    }
    
    ss << suffix;
    
    return ss.str();
  }
};

std::string LazyArray::svalue() const
{
  std::string s("(? ");
  s += data.code()->svalue();
  s += " )";
  return s;
}

/*
std::string Map::svalue() const
{
  if (value->empty())
    return "{:}";
  
  std::stringstream ss(std::stringstream::out);
  
  bool first = true;
  
  ss << "{";
  for (std::unordered_map<Value*, Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << it->first->svalue() << ":" << it->second->svalue();
  }
  
  ss << "}";
  
  return ss.str();
}*/

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
