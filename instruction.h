/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "defines.h"
#include "Value.h"
#include "CompositeValue.h"
#include "code.h"

#include <string>

class VM;
class Code;

class Instruction
{
  public:
    Instruction() { }
    virtual void execute(VM *vm) const = 0;
    virtual bool equals(Instruction *) const { return false; } 
  
    virtual std::string svalue() const = 0;
};

class Value;

class PushInstruction : public Instruction
{
  private:
    Value value;

  public:
    PushInstruction(Value& value) : Instruction(), value(value) { }
    PushInstruction(Value&& value) : Instruction(), value(value) { }
    virtual void execute(VM *vm) const;
    virtual std::string svalue() const;
};

class OpcodeInstruction : public Instruction
{
public:
  const Opcode opcode;

public:
  OpcodeInstruction(Opcode opcode) : Instruction(), opcode(opcode) {}
  virtual void execute(VM *vm) const;
  virtual std::string svalue() const;
};

#endif
