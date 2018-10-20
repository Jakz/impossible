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

using namespace std;

class LazyArrayHolder
{
  private:
    vector<Value*>* values;
    Lambda *lambda;
  
    unordered_map<u32, Lambda*> indices;
    const bool useIndices;
  
  public:
    LazyArrayHolder(Lambda *lambda, bool useIndices) : values(new std::vector<Value*>()), lambda(lambda), useIndices(useIndices) { }
    //LazyArray(Lambda *lambda, u32 index) : values(new vector<Value*>()), lambda(lambda) { generateUpTo(index); }
  
    void shrinkBy(u32 i) { values->resize(values->size()-1); }
  
  
    void addMap(u32 i, Lambda* l) { indices[i] = l; }
  
    void generateUpTo(VM* vm, u32 index);
    void generateNth(VM* vm, u32 index);

    Value *at(VM* vm, u32 index);
    u32 index;
  
    Lambda *code() const { return lambda; }
    //vector<Value*>* data() const { return values; }
};


#endif
