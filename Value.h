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

#include <cmath>

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define TYPES(x, y) (x << 4 | y)

enum Type : s16
{
  TYPE_INVALID = -1,
  
  TYPE_NIL = 0,
  TYPE_INT,
  TYPE_FLOAT,
  
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_RANGE,
  
  TYPE_LIST,
  TYPE_ARRAY,
  TYPE_SET,
  TYPE_MAP,
  TYPE_STACK,
  TYPE_QUEUE,
  TYPE_LAZY_ARRAY,
  
  TYPE_LAMBDA,
  TYPE_GENERIC,

  
  TYPES_COUNT,
  
  TYPE_GENERIC2 = 500,
  TYPE_GENERIC3,
  TYPE_UNKNOWN,
  
  TYPE_NONE
};

class Value
{
  public:
    Value(Type type, bool iterable) : type(type), iterable(iterable) { }

    virtual string svalue() const = 0;
    virtual string lvalue();
  
    virtual bool equals(const Value *value) const = 0;
    virtual Value* clone() const = 0;
  
    const Type type;
    const bool iterable;

    virtual ~Value() { };
  
    template<typename T> T* as() { return static_cast<T*>(this); }
  
    static bool isCollection(Type type)
    {
      switch (type)
      {
        case TYPE_LIST:
        case TYPE_ARRAY:
        case TYPE_SET:
        case TYPE_MAP:
        case TYPE_STACK:
        case TYPE_QUEUE:
        case TYPE_RANGE:
        case TYPE_LAZY_ARRAY:
        case TYPE_STRING:
          return true;
        default:
          return false;
      }
    }
  
    static const char* typeName(Type type)
    { 
      switch (type)
      {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOL: return "bool";
        case TYPE_CHAR: return "char";
        case TYPE_STRING: return "string";
        case TYPE_RANGE: return "range";
        case TYPE_LIST: return "list";
        case TYPE_ARRAY: return "array";

        case TYPE_SET: return "set";
        case TYPE_STACK: return "stack";
        case TYPE_QUEUE: return "queue";
          
        case TYPE_LAZY_ARRAY: return "larray";
          
        case TYPE_MAP: return "map";
        case TYPE_LAMBDA: return "lambda";
        case TYPE_NIL: return "nil";
          
        case TYPE_GENERIC: return "A";
        case TYPE_GENERIC2: return "B";
        case TYPE_GENERIC3: return "C";
        case TYPE_UNKNOWN: return "?";
          
        case TYPES_COUNT:
        case TYPE_NONE: return "NONE";
        case TYPE_INVALID: return "ERROR";
      }
    }
  
    static Type typeValue(string string)
    {
      if (string.compare("int") == 0) return TYPE_INT;
      else if (string.compare("float") == 0) return TYPE_FLOAT;
      else if (string.compare("bool") == 0) return TYPE_BOOL;
      else if (string.compare("char") == 0) return TYPE_CHAR;
      else if (string.compare("string") == 0) return TYPE_STRING;
      else if (string.compare("range") == 0) return TYPE_RANGE;
      
      else if (string.compare("list") == 0) return TYPE_LIST;
      else if (string.compare("array") == 0) return TYPE_ARRAY;
      else if (string.compare("set") == 0) return TYPE_SET;
      else if (string.compare("queue") == 0) return TYPE_QUEUE;
      else if (string.compare("stack") == 0) return TYPE_STACK;
      
      else if (string.compare("larray") == 0) return TYPE_LAZY_ARRAY;
      
      else if (string.compare("map") == 0) return TYPE_MAP;
      else if (string.compare("lambda") == 0) return TYPE_LAMBDA;
      
      
      else if (string.compare("nil") == 0) return TYPE_NIL;
      
      
      else if (string.compare("A") == 0) return TYPE_GENERIC;
      else if (string.compare("?") == 0) return TYPE_UNKNOWN;
      else return TYPE_INVALID;
    }
};

class TCollection
{
  public:
    virtual void iterate() = 0;
    virtual bool hasNext() = 0;
    virtual Value *next() = 0;
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
    TValue(Type type, bool iterable, T value) : Value(type, iterable), value(value) {}
    
    void set(T value) { this->value = value; }
    T get() const { return this->value; }
};

template <>
class TValue<void*> : public Value
{
  public:
    TValue(Type type) : Value(TYPE_NIL, false) {}
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
