/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#ifndef _CODE_H_
#define _CODE_H_

#include "defines.h"

#include "instruction.h"

#include <vector>
#include <string>

class Instruction;
class Value;

class Code
{
public:
  virtual size_t size() const = 0;
  virtual const Instruction& at(size_t i) const = 0;
  
  virtual Code* append(const Instruction& ins) = 0;
  
  std::string svalue() const;
  virtual std::string svalue(size_t pc) = 0;
};

class CodeStandard : public Code
{
private:
  std::vector<Instruction> code;
  
public:
  CodeStandard(const std::vector<Instruction>& code) : code(code) { }
  
  CodeStandard(size_t length) { code.resize(length); }
  CodeStandard(const Instruction& i) { code.push_back(i); }
  
  virtual Code *append(const Instruction& ins)
  {
    auto code = this->code;
    code.push_back(ins);
    return new CodeStandard(code);
  }
  
  virtual std::string svalue(size_t pc);
  
  virtual const Instruction& at(size_t i) const { return code[i]; }
  virtual size_t size() const { return code.size(); }
};

#endif
