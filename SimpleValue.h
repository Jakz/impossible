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

class Char : public TValue<char>
{
private:
  
public:
  Char(char value) : TValue<char>(TYPE_CHAR, value) { }
  
  virtual string svalue() const;
  
  virtual bool equals(const Value *value) const { return this->value == ((TValue<char>*)value)->get(); }
  virtual Value *clone() const { return new Float(value); }
};

class Bool : public TValue<bool>
{
private:
  
public:
  Bool(bool value) : TValue<bool>(TYPE_BOOL, value) { }
  
  virtual string svalue() const;
  
  virtual bool equals(const Value *value) const { return this->value == ((TValue<bool>*)value)->get(); }
  virtual Value *clone() const { return new Bool(value); }
};


#endif
