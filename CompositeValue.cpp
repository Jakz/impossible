//
//  CompositeTypes.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#include "CompositeValue.h"
#include "TypeUtils.h"
#include "instruction.h"

#include <iostream>
#include <iomanip>
#include <sstream>




Value *String::next()
{
  if (it == value.end())
    return NULL;
  else
  {
    char v = *it++;
    return new Char(v);
  }
}

string Range::svalue() const
{
  vector<RangePair>::const_iterator it;
  
  stringstream ss(stringstream::out);
  
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

string List::svalue() const
{
  stringstream ss(stringstream::out);
  
  ss << "{";
  
  bool first = true;
  
  for (list<Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << (*it)->svalue();
  }
  
  ss << "}";
  
  return ss.str();
}

string Stack::svalue() const
{
  stringstream ss(stringstream::out);
  
  ss << "{>";
  
  bool first = true;
  
  for (list<Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << (*it)->svalue();
  }
  
  ss << "}";
  
  return ss.str();
}

string Queue::svalue() const
{
  stringstream ss(stringstream::out);
  
  ss << "{<";
  
  bool first = true;
  
  for (list<Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << (*it)->svalue();
  }
  
  ss << "}";
  
  return ss.str();
}

string Array::svalue() const
{
  stringstream ss(stringstream::out);
  
  bool first = true;
  
  ss << "(";
  for (vector<Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << (*it)->svalue();
    
  }
  
  ss << ")";
  
  return ss.str();
}

string LazyArray::svalue() const
{
  string s("(? ");
  s += value.code()->svalue();
  s += " )";
  return s;
}

string Set::svalue() const
{
  stringstream ss(stringstream::out);
  
  ss << "{. ";
  
  bool first = true;
  
  for (unordered_set<Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
  {
    if (first)
      first = false;
    else
      ss << " ";
    
    ss << (*it)->svalue();
  }
  
  ss << "}";
  
  return ss.str();
}

string Map::svalue() const
{
  if (value->empty())
    return "{:}";
  
  stringstream ss(stringstream::out);
  
  bool first = true;
  
  ss << "{";
  for (unordered_map<Value*, Value*>::const_iterator it = value->begin(); it != value->end(); ++it)
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

string Lambda::svalue() const
{
  stringstream ss(stringstream::out);
  
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

bool List::equals(const Value *value) const
{
  if (value->type == TYPE_LIST)
  {
    list<Value*>* cvalue = ((List*)value)->value;
    
    list<Value*>::const_iterator it, it2;
    
    for (it = this->value->begin(), it2 = cvalue->begin(); it != this->value->end() && it2 != cvalue->end(); ++it, ++it2)
      if (!(*it)->equals((*it2)))
        return false;
    
    return it == this->value->end() && it2 == cvalue->end();
  }
  
  return false;
}

Value *List::clone() const
{
  /*list<Value*>::const_iterator it;
   list<Value*> nvalues;
   
   for (it = value.begin(); it != value.end(); ++it)
   nvalues.push_back((*it)->clone());
   
   return new List(nvalues);*/
  return new List(value);
}

bool Stack::equals(const Value *value) const
{
  if (value->type == TYPE_STACK)
  {
    list<Value*>* cvalue = ((Stack*)value)->value;
    
    list<Value*>::const_iterator it, it2;
    
    for (it = this->value->begin(), it2 = cvalue->begin(); it != this->value->end() && it2 != cvalue->end(); ++it, ++it2)
      if (!(*it)->equals((*it2)))
        return false;
    
    return it == this->value->end() && it2 == cvalue->end();
  }
  
  return false;
}

Value *Stack::clone() const
{
  return new Stack(value);
}

bool Queue::equals(const Value *value) const
{
  if (value->type == TYPE_QUEUE)
  {
    list<Value*>* cvalue = ((Stack*)value)->get();
    
    list<Value*>::const_iterator it, it2;
    
    for (it = this->value->begin(), it2 = cvalue->begin(); it != this->value->end() && it2 != cvalue->end(); ++it, ++it2)
      if (!(*it)->equals((*it2)))
        return false;
    
    return it == this->value->end() && it2 == cvalue->end();
  }
  
  return false;
}

Value *Queue::clone() const
{
  return new Queue(value);
}

bool Array::equals(const Value *value) const
{
  if (value->type == TYPE_ARRAY)
  {
    vector<Value*>* cvalue = ((Array*)value)->value;
    
    vector<Value*>::const_iterator it, it2;
    
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
    unordered_set<Value*>* cvalue = ((Set*)value)->value;
    
    if (cvalue->size()== this->value->size())
    {
      unordered_set<Value*>::iterator it, it2;
      
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

