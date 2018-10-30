/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#ifndef _VALUE_H_
#define _VALUE_H_

#include "defines.h"
#include "vm/heap.h"

#include "types/traits.h"

#include "types/range.h"
#include "types/lazy.h"

#include <cmath>

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#define TYPES(x, y) (x << 8 | y)

using opcode_t = Opcode;
class TCollection;
class String;
class Regex;
class Tuple;
class Array;
class List;
class Stack;
class Queue;
class Set;
class Map;
class Range;
class LazyArray;
class Code;
class Error;

union value_data
{
  opcode_t op;
  integral_t i;
  real_t f;
  char c;
  bool b;
  managed_object* ptr;
  
  value_data(opcode_t op) : op(op) { }
  value_data(integral_t i) : i(i) { }
  value_data(real_t f) : f(f) { }
  value_data(char c) : c(c) { }
  value_data(bool b) : b(b) { }
  
  value_data(managed_object* ptr) : ptr(ptr) { }
  
  bool operator==(const value_data& o) const { return i == o.i; }
};

#if defined(USE_PACKED_VALUES) && USE_PACKED_VALUES
class __attribute__((packed)) Value final
#else
class Value final
#endif
{
public:
  value_data data;
  TypeInfo type;

public:
  Value() : type(TYPE_NIL), data(0LL) { }
  Value(const Value& other) : type(other.type), data(other.data) { }
  
  Value(Type type) : type(type), data(nullptr) { }
  Value(Type type, managed_object* ptr) : type(type), data(ptr) { }
  
  //Value(int value) : Value((integral_t)value) { }
  Value(integral_t value) : type(TYPE_INT), data(value) { }
  Value(real_t value) : type(TYPE_FLOAT), data(value) { }
  Value(char value) : type(TYPE_CHAR), data(value) { }
  Value(bool value) : type(TYPE_BOOL), data(value) { }
  Value(opcode_t value) : type(TYPE_OPCODE), data(value) { }
  
  Value(String* string);
  Value(Regex* string);
  Value(Tuple* tuple);
  Value(List* list);
  Value(Stack* list);
  Value(Queue* list);
  Value(Array* array);
  Value(Map* map);
  Value(Set* set);
  Value(Lambda* lambda);
  Value(Range* range);
  Value(LazyArray* lazyArray);
  
  Value(Error* error);
  
  Value& operator=(const Value& other) { this->data = other.data; this->type = other.type; return *this; }
  bool valid() const { return type != TYPE_INVALID; }
  
  bool operator==(const Value& value) const { return type.traits().equal_to(*this, value); }
  std::string svalue() const { return type.traits().to_string(*this); }
  bool equals(const Value& value) const { return this->operator==(value); }

  integral_t integral() const { return data.i; }
  real_t real() const { return data.f; }
  char character() const { return data.c; }
  bool boolean() const { return data.b; }
  opcode_t opcode() const { return data.op; }
  
  template<typename T> auto number() const -> typename std::enable_if<std::is_same<T, integral_t>::value, T>::type { return data.i; }
  template<typename T> auto number() const -> typename std::enable_if<std::is_same<T, real_t>::value, T>::type { return data.f; }

  
  template<typename T> T* object() const
  {
    T* ptr = dynamic_cast<T*>(data.ptr);
    assert(ptr);
    return ptr;
  }
  
  TCollection* collection() const; //TODO: temporarily virtual to override in collections
  
  String* string() const;
  Regex* regex() const;
  Tuple* tuple() const;
  List* list() const;
  Stack* stack() const;
  Queue* queue() const;
  Set* set() const;
  Map* map() const;
  Array* array() const;
  Range* range() const;
  LazyArray* lazyArray() const;
  Error* error() const;
  
  Traits::Indexable* indexable() const;
  Traits::Iterable* iterable() const;

  Lambda* lambda() const;
  
  
  static Value INVALID;
};

class TCollection : public managed_object, public Traits::Countable
{
public:
  virtual void iterate() const = 0;
  virtual bool hasNext() const = 0;

  virtual const Value& next() const = 0;

  virtual void put(Value value) = 0;
  virtual bool empty() const = 0;
  
  //TODO: must override for map
  virtual bool contains(const Value& value)
  {
    iterate();
    
    while (hasNext())
    {
      if (next() == value)
        return true;
    }
    
    return false;
  }
};

#endif
