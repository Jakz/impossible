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

static const CollectionPrinter<Value*> ArrayPrinter = { "(", ")", " ", [] (const Value* v) { return v->svalue(); } };
static const CollectionPrinter<Value*> SetPrinter = { "{.", "}", " ", [] (const Value* v) { return v->svalue(); } };

std::string Array::svalue() const { return ArrayPrinter.svalue(this); }
std::string Set::svalue() const { return SetPrinter.svalue(this); }


std::string LazyArray::svalue() const
{
  std::string s("(? ");
  s += value.code()->svalue();
  s += " )";
  return s;
}


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
}

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
bool Array::equals(const Value *value) const
{
  if (value->type == TYPE_ARRAY)
  {
    std::vector<Value*>* cvalue = ((Array*)value)->value;
    
    std::vector<Value*>::const_iterator it, it2;
    
    for (it = this->value->begin(), it2 = cvalue->begin(); it != this->value->end() && it2 != cvalue->end(); ++it, ++it2)
      if (!(*it)->equals((*it2)))
        return false;
    
    return it == this->value->end() && it2 == cvalue->end();
  }
  
  return false;
}

Value *Array::clone() const
{
  /*vector<Value*>::const_iterator it;
   vector<Value*> nvalues;
   
   for (it = value.begin(); it != value.end(); ++it)
   nvalues.push_back((*it)->clone());
   
   return new Array(nvalues);*/
  return new Array(value);
}

bool Set::equals(const Value *value) const
{
  if (value->type == TYPE_SET)
  {
    std::unordered_set<Value*>* cvalue = ((Set*)value)->value;
    
    if (cvalue->size()== this->value->size())
    {
      std::unordered_set<Value*>::iterator it, it2;
      
      for (it = this->value->begin(); it != this->value->end(); ++it)
      {
        bool found = false;
        for (it2 = cvalue->begin(); it2 != cvalue->end(); ++it2)
        {
          if ((*it)->equals(*it2))
          {
            found = true;
            break;
          }
        }
        
        if (!found)
          return false;
      }
      
      return true;
    }
  }
  
  return false;
}

Value *Set::clone() const
{
  return new Set(value);
}
