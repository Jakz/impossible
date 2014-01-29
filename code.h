/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#ifndef _CODE_H_
#define _CODE_H_

#include "defines.h"
#include <string>

class Instruction;
class PushInstruction;
class Value;

class Code
{
  public:
    virtual u32 len() = 0;
    virtual void set(u32 i, Instruction *it) = 0;
    virtual Instruction* at(u32 i) = 0;
    virtual Code *append(Instruction *ins) = 0;
  
    virtual std::string svalue(u32 pc) = 0;
};

class CodeStandard : public Code
{
  private:
    Instruction** code;
    const u32 length;
    
  public:
    CodeStandard(Instruction **code, const u32 length) : code(code), length(length) { };
    CodeStandard(const u32 length) : code(new Instruction*[length]), length(length) { };
    CodeStandard(Instruction *i) : code(new Instruction*[1]), length(1) { code[0] = i; };
    
    virtual Code *append(Instruction *ins)
    {
      CodeStandard *c = new CodeStandard(this->length+1);
      for (int i = 0; i < this->length; ++i)
        c->code[i] = this->code[i];
      
      c->code[this->length] = ins;
      
      return c;
    }
  
    virtual std::string svalue(u32 pc);

    
    virtual void set(u32 i, Instruction *is) { code[i] = is; }
    virtual Instruction* at(u32 i) { return code[i]; }
    virtual u32 len() { return length; }
  
  friend class CurriedCode;
};

class CurriedCode : public Code
{
  private:
    Code *code;
    PushInstruction *value;
    
  public:
    CurriedCode(Code *code, Value *value);
    CurriedCode(Code *code, PushInstruction *value);
  
    virtual Code *append(Instruction *ins);
  
    virtual void set(u32 i, Instruction *is);
    virtual Instruction* at(u32 i);
    virtual u32 len();
  
    virtual std::string svalue(u32 pc) { return ""; }
    
};

#endif
