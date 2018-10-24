/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#include "code.h"
#include "instruction.h"
#include "value.h"
#include "collection.h"

#include <iostream>
#include <iomanip>
#include <sstream>

std::string Code::svalue() const
{
  std::stringstream ss(std::stringstream::out);
  size_t size = this->size();
  
  ss << "[";
  
  for (size_t i = 0; i < size; ++i)
  {
    const Instruction* in = at(i);
    ss << in->svalue();
  }
  
  ss << "]";
  
  return ss.str();
}

std::string CodeStandard::svalue(size_t pc)
{
  std::stringstream ss(std::stringstream::out);
  size_t l = 0;
  
  for (int i = 0; i < size(); ++i)
  {
    ss << code[i]->svalue() << " ";
    if (i+1 == pc)
      l = ss.str().length();
    
  }
  
  ss << std::endl;
  ss << std::string(l, ' ');
  ss << '^';
  
  
  return ss.str();
}


CurriedCode::CurriedCode(Code *code, Value value) : code(code), value(new PushInstruction(value)) { };
CurriedCode::CurriedCode(Code *code, PushInstruction *value) : code(code), value(value) { };

size_t CurriedCode::size() const { return code->size() + 1; }

Instruction *CurriedCode::at(size_t i) const
{
  if (i == 0)
    return value;
  else
    return code->at(i-1);
}

void CurriedCode::set(size_t i, Instruction *is)
{
  code->set(i-1, is);
}

Code *CurriedCode::append(Instruction *ins)
{
  return new CurriedCode(code->append(ins), value);
}
