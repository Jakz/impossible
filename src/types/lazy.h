/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#ifndef _LAZY_H_
#define _LAZY_H_

#include "defines.h"
#include <vector>
#include <unordered_map>

class Value;
class Lambda;
class VM;

class LazyArrayHolder
{
public:
  using data_t = std::vector<Value>;
  
private:
  mutable data_t values;
  Lambda *lambda;
  
  std::unordered_map<integral_t, Lambda*> indices;
  const bool useIndices;
  
public:
  LazyArrayHolder(Lambda* lambda, bool useIndices) : lambda(lambda), useIndices(useIndices) { }
  
  void shrinkBy(integral_t i) { values.resize(values.size()-1); }
  
  
  void addMap(integral_t i, Lambda* l) { indices[i] = l; }
  
  void generateUpTo(VM* vm, integral_t index);
  void generateNth(VM* vm, integral_t index) const;
  //TODO: must become reference
  const Value& at(VM* vm, integral_t index) const;
  mutable integral_t index;
  
  Lambda *code() const { return lambda; }
  const data_t& data() const { return values; }
  size_t size() const { return values.size(); }
  
  data_t::const_iterator begin() const { return values.begin(); }
  data_t::const_iterator end() const { return values.end(); }
};


#endif
