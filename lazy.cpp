/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#include "lazy.h"

#include "Values.h"
#include "vm.h"

void LazyArrayHolder::generateUpTo(VM* vm, u32 index)
{
  bool check = false;
  
  if (values->size() <= index)
  {
    values->resize(index+1, NULL);
    check = true;
  }
  else
  {
    for (int i = 0; i < index; ++i)
      if (!values->at(i))
      {
        check = true;
        break;
      }
  }
  
  if (check)
  {
    for (int i = 0; i < index; ++i)
      if (!values->at(i))
        generateNth(vm, i);
  }
}

void LazyArrayHolder::generateNth(VM* vm, u32 index)
{
  //std::cout << "generating lazy at " << index << endl;

  if (useIndices)
    vm->push(new Int(index));
  
  unordered_map<u32, Lambda*>::iterator it = indices.find(index);
  
  vm->lazy = this;
  this->index = index;
  
  if (it != indices.end())
    vm->execute(it->second->get());
  else
    vm->execute(lambda->get());
  
  Value *v;
  vm->popOne(&v);
  //std::cout << "lazy at " << index << " -> " << v->svalue() << endl;
  (*values)[index] = v;
  
  vm->lazy = NULL;
}

Value *LazyArrayHolder::at(VM* vm, u32 index)
{
  //std::cout << "request for lazy at " << index << endl;
  
  Value *v = NULL;
  
  if (values->size() <= index)
  {
    values->resize(index+1, NULL);
  }
  else
  {
    v = values->at(index);
  }

  if (v) return v;
  else
  {
    generateNth(vm, index);
    return values->at(index);
  }
}