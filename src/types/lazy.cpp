/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#include "lazy.h"

#include "value.h"
#include "collection.h"

#include "vm.h"

void LazyArrayHolder::generateUpTo(VM* vm, integral_t index)
{
  bool check = false;
  
  if (values.size() <= index)
  {
    values.resize(index+1, TYPE_INVALID);
    check = true;
  }
  else
  {
    for (size_t i = 0; i < index; ++i)
      if (!values[i].valid())
      {
        check = true;
        break;
      }
  }
  
  if (check)
  {
    for (size_t i = 0; i < index; ++i)
      if (!values[i].valid())
        generateNth(vm, i);
  }
}

void LazyArrayHolder::generateNth(VM* vm, integral_t index) const
{
  std::cout << "generating lazy at " << index << std::endl;

  if (useIndices)
    vm->push(index);
  
  auto it = indices.find(index);
  
  //TODO: hack, make self-reentrant
  vm->pushContext(Context(this, index));
  
  if (it != indices.end())
    vm->execute(it->second->code());
  else
    vm->execute(lambda->code());
  
  vm->popOne(values[index]);
  vm->popContext();
}

const Value& LazyArrayHolder::at(VM* vm, integral_t index) const
{
  std::cout << "request for lazy at " << index << std::endl;
  
  if (values.size() <= index)
  {
    values.resize(index+1, TYPE_INVALID);
    generateNth(vm, index);
  }

  auto& value = values[index];

  if (!value.valid())
    generateNth(vm, index);
  
  return values[index];
}
