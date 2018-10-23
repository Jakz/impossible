/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#ifndef _VALUE_H_
#define _VALUE_H_

#include "defines.h"
#include "range.h"
#include "code.h"
#include "lazy.h"

#include "TypeTraits.h"

#include <cmath>

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#define TYPES(x, y) (x << 4 | y)

struct managed_object
{
  virtual ~managed_object() { }
};

class String;

union value_data
{
  integral_t i;
  real_t f;
  char c;
  bool b;
  managed_object* ptr;
  
  value_data(integral_t i) : i(i) { }
  value_data(real_t f) : f(f) { }
  value_data(char c) : c(c) { }
  value_data(bool b) : b(b) { }
  
  value_data(managed_object* ptr) : ptr(ptr) { }
  
  bool operator==(const value_data& o) const { return i == o.i; }
};

class Value
{
public:
  value_data data;

public:
  Value(const Value& other) : type(other.type), data(other.data) { }
  
  Value(Type type) : type(type), data(nullptr) { }
  Value(Type type, managed_object* ptr) : type(type), data(ptr) { }
  
  Value(integral_t value) : type(TYPE_INT), data(value) { }
  Value(real_t value) : type(TYPE_FLOAT), data(value) { }
  Value(char value) : type(TYPE_CHAR), data(value) { }
  Value(bool value) : type(TYPE_BOOL), data(value) { }
  
  Value(String* string);
  
  virtual std::string svalue() const { return type.traits().to_string(*this); }
  std::string lvalue();

  virtual bool equals(const Value *value) const { return type.traits().equal_to(*this, *value); }
  virtual Value* clone() const { return new Value(*this); }

  const TypeInfo type;

  virtual ~Value() { };

  template<typename T> auto as() const -> typename std::enable_if<std::is_integral<T>::value, T>::type { return static_cast<T>(data.i); }
  template<typename T> auto as() -> typename std::enable_if<std::is_pointer<T>::value, T>::type { return reinterpret_cast<T>(this); }
  
  integral_t integral() const { return data.i; }
  real_t real() const { return data.f; }
  char character() const { return data.c; }
  bool boolean() const { return data.b; }
  
  template<typename T> T* object() const { return static_cast<T*>(data.ptr); }
  
  String* string() const;
};

class TCollection : public managed_object
{
  public:
    virtual void iterate() const = 0;
    virtual bool hasNext() const = 0;
  
    virtual Value *next() const = 0;

    virtual void put(Value *value) = 0;
    virtual u32 size() const = 0;
    virtual bool empty() const = 0;
};


template <class T>
class TValue : public Value
{
  protected:
    T value;
    
  public:
    TValue(Type type, T value) : Value(type), value(value) {}
    
    void set(T value) { this->value = value; }
    T get() const { return this->value; }
};

template <>
class TValue<void*> : public Value
{
  public:
    TValue(Type type) : Value(TYPE_NIL) {}
    //TValue(const TValue &o) { }
    //TValue &operator= (const TValue &o) { }
  
    virtual std::string svalue() const { return "nil"; }
    
    virtual bool equals(const Value *value) const { return value->type == TYPE_NIL; }
    virtual Value* clone() const { return (Value*)this; }
    
    void set(void* value) { }
    void* get() const { return NULL; }
  
    static const Value *NIL;
};

class Heap
{
private:
  using refcount_t = u16;
  
  struct heap_object
  {
    managed_object* value;
    mutable refcount_t refs;
    
    heap_object(managed_object* value ) : value(value), refs(0) { }
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


#endif
