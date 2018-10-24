/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#ifndef _CODE_H_
#define _CODE_H_

#include "defines.h"

#include <vector>
#include <string>

class Instruction;
class PushInstruction;
class Value;

class Code
{
public:
  virtual size_t len() = 0;
  virtual void set(size_t i, Instruction *it) = 0;
  virtual Instruction* at(size_t i) = 0;
  virtual Code *append(Instruction *ins) = 0;
  
  virtual std::string svalue(size_t pc) = 0;
};

class CodeStandard : public Code
{
private:
  std::vector<Instruction*> code;
  
public:
  CodeStandard(const std::vector<Instruction*>& code) : code(code) { }
  
  CodeStandard(Instruction **code, size_t length) {
    std::copy(code, code + length, std::back_inserter(this->code));
  }
  
  CodeStandard(size_t length) { code.resize(length); }
  CodeStandard(Instruction* i) { code.push_back(i); }
  
  virtual Code *append(Instruction *ins)
  {
    auto code = this->code;
    code.push_back(ins);
    return new CodeStandard(code);
  }
  
  virtual std::string svalue(size_t pc);
  
  virtual void set(size_t i, Instruction *is) { code[i] = is; }
  virtual Instruction* at(size_t i) { return code[i]; }
  virtual size_t len() { return code.size(); }
  
  friend class CurriedCode;
};

class CurriedCode : public Code
{
private:
  Code *code;
  PushInstruction *value;
  
public:
  CurriedCode(Code *code, Value value);
  CurriedCode(Code *code, PushInstruction *value);
  
  virtual Code *append(Instruction *ins);
  
  virtual void set(size_t i, Instruction *is);
  virtual Instruction* at(size_t i);
  virtual size_t len();
  
  virtual std::string svalue(size_t pc) { return ""; }
  
};

#endif
