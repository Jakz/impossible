/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#pragma once

#include "defines.h"
#include "value.h"
#include "collection.h"
#include "code.h"

#include <string>

class VM;
class Code;

class Instruction final
{
private:
  
  union
  {
    Value _value;
    Opcode _opcode;
  };
  
  bool isPush;
  
public:
  Instruction() { }
  Instruction(Opcode opcode) : _opcode(opcode), isPush(false) { }
  Instruction(Value&& value) : _value(value), isPush(true) { }
  Instruction(const Value& value) : _value(value), isPush(true) { }
  
  Instruction(const Instruction& other)
  {
    isPush = other.isPush;
    if (isPush) _value = other._value;
    else _opcode = other._opcode;
  }
  
  Instruction(const Instruction&& other)
  {
    isPush = other.isPush;
    if (isPush) _value = std::move(other._value);
    else _opcode = other._opcode;
  }
  
  Instruction& operator=(const Instruction& other)
  {
    isPush = other.isPush;
    if (isPush) _value = other._value;
    else _opcode = other._opcode;
    return *this;
  }
  
  ~Instruction() { if (isPush) _value.~Value(); }
  
  void execute(VM *vm) const;
  std::string svalue() const;
  
  Opcode opcode() const { return _opcode; }
  const Value& value() const { return _value; }
  
};
