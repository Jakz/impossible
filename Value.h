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

using namespace std;

#define TYPES(x, y) (x << 4 | y)

union value_data
{
  s64 i;
  double f;
  char c;
  bool b;
  void* ptr;
  
  value_data(s64 i) : i(i) { }
  value_data(void* ptr) : ptr(nullptr) { }
  
  bool operator==(const value_data& o) const { return i == o.i; }
};

class Value
{
public:
  value_data data;

public:
  Value(Value& other) : type(other.type), data(other.data) { }
  
  Value(Type type) : type(type), data(nullptr) { }
  
  Value(s64 value) : type(TYPE_INT), data(value) { }

  virtual string svalue() const = 0;
  virtual string lvalue();

  virtual bool equals(const Value *value) const = 0;
  virtual Value* clone() const = 0;

  const TypeInfo type;

  virtual ~Value() { };

  template<typename T> auto as() const -> typename std::enable_if<std::is_integral<T>::value, T>::type { return static_cast<T>(data.i); }
  template<typename T> auto as() -> typename std::enable_if<std::is_pointer<T>::value, T>::type { return reinterpret_cast<T>(this); }
  
  integral integral() const { return data.i; }
};

class TCollection
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
  
    virtual string svalue() const { return "nil"; }
    virtual string lvalue() { return "nil"; }
    
    virtual bool equals(const Value *value) const { return value->type == TYPE_NIL; }
    virtual Value* clone() const { return (Value*)this; }
    
    void set(void* value) { }
    void* get() const { return NULL; }
  
    static const Value *NIL;
};


#endif
