//
//  integral_types.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#ifndef _SIMPLE_TYPES_H_
#define _SIMPLE_TYPES_H_

#include "Value.h"

class Int : public Value
{
private:
  
public:
  Int(integral_t value) : Value(value) { }
  virtual Value *clone() const { return new Int(data.i); }
};

class Float : public Value
{
private:
  
public:
  Float(real_t value) : Value(value) { }
  virtual Value *clone() const { return new Float(data.f); }
};

class Char : public Value
{
private:
  
public:
  Char(char value) : Value(value) { }
  virtual Value *clone() const { return new Char(data.c); }
};

class Bool : public Value
{
private:
  
public:
  Bool(bool value) : Value(value) { }
  virtual Value *clone() const { return new Bool(data.b); }
};

class Nil : public Value
{
public:
  Nil() : Value(TYPE_NIL, nullptr) { }
  virtual Value* clone() const { return new Nil(); }
};


#endif
