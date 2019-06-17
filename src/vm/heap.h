#pragma once

#include "defines.h"

#include <vector>

struct managed_object
{
  virtual ~managed_object() { }
};

class Heap
{
private:
  using refcount_t = u16;
  
  struct heap_object
  {
    managed_object* value;
    mutable refcount_t refs;
    
    heap_object(managed_object* value) : value(value), refs(0) { }
    void retain() const { ++refs; }
    bool release() const { return --refs == 0; }
  };
  
private:
  std::vector<heap_object> heap;
  
public:
  
  void insert(managed_object* value)
  {
    heap.emplace_back(value);
  }
};
