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

Value* String::next() const
{
  if (it == value.end())
    return NULL;
  else
  {
    char v = *it++;
    return new Char(v);
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

std::string Range::svalue() const
{
  std::vector<RangePair>::const_iterator it;
  
  std::stringstream ss(std::stringstream::out);
  
  bool first = true;
  for (it = value.data->begin(); it != value.data->end(); ++it)
  {
    const RangePair &r = *it;
    
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << r.a << ".." << r.b;
  }
  
  return ss.str();
}

std::string LazyArray::svalue() const
{
  std::string s("(? ");
  s += value.code()->svalue();
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
  
  for (int i = 0; i < value->len(); ++i)
  {
    ss << value->at(i)->svalue();
    if (i < value->len() - 1)
      ss << " ";
  }
  
  ss << "]";
  
  return ss.str();
}