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
  Int(s64 value) : Value(value) { }
  Int(int value) : Value(value) { }
  
  virtual bool equals(const Value* value) const { return type == value->type && data == value->data; }
  virtual Value *clone() const { return new Int(data.i); }
};

class Float : public TValue<double>
{
private:
  
public:
  Float(float value) : TValue<double>(TYPE_FLOAT, value) { }
  
  virtual string svalue() const;
  
  virtual bool equals(const Value *value) const { return this->value == ((TValue<double>*)value)->get(); }
  virtual Value *clone() const { return new Float(value); }
};

class Char : public TValue<char>
{
private:
  
public:
  Char(float value) : TValue<char>(TYPE_CHAR, value) { }
  
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
