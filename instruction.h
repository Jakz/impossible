/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "defines.h"
#include "Values.h"
#include "code.h"

#include <string>

using namespace std;

class VM;

struct Code;

Code *assemble(const char *string);

class Instruction
{
  public:
    Instruction(bool verbose) : verbose(verbose) { }
    virtual void execute(VM *vm) const = 0;
    virtual bool equals(Instruction *) const { return false; } 
  
    virtual string svalue() const = 0;
  
    const bool verbose;
};

class Value;

class PushInstruction : public Instruction
{
  private:
    Value *value;

  public:
    PushInstruction(Value *value) : Instruction(false), value(value) { }
    virtual void execute(VM *vm) const;
    virtual string svalue() const;
};

class OpcodeInstruction : public Instruction
{
  Opcode opcode;

  public:
    OpcodeInstruction(Opcode opcode) : Instruction(true), opcode(opcode) {}
    virtual void execute(VM *vm) const;
    virtual string svalue() const;
};

struct InstructionTree
{
  Instruction *i;
  InstructionTree *n;
  u32 depth;
  
  InstructionTree(Instruction *i, InstructionTree *il)
  {
    this->i = i;
    this->n = il;
    
    this->depth = il ? il->depth + 1 : 1;
  }
  
  Code* assemble();
};

#endif
