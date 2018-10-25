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
  Value _value;

public:
  Instruction() { }
  Instruction(Opcode opcode) : _value(opcode) { }
  Instruction(Value&& value) : _value(value) { }
  Instruction(const Value& value) : _value(value) { }
  
  void execute(VM *vm) const;
  std::string svalue() const;
  
  bool isValue() const { return _value.type != TYPE_OPCODE; }
  
  Opcode opcode() const { return _value.opcode(); }
  const Value& value() const { return _value; }
  
};
