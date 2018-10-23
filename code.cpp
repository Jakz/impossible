/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/4/13
 */

#include "code.h"
#include "instruction.h"
#include "Values.h"

#include <iostream>
#include <iomanip>
#include <sstream>

std::string CodeStandard::svalue(u32 pc)
{
  std::stringstream ss(std::stringstream::out);
  u32 l = 0;
  
  for (int i = 0; i < len(); ++i)
  {
    ss << code[i]->svalue() << " ";
    if (i+1 == pc)
      l = (u32)ss.str().length();
    
  }
  
  ss << std::endl;
  ss << std::string(l, ' ');
  ss << '^';
  
  
  return ss.str();
}


CurriedCode::CurriedCode(Code *code, Value *value) : code(code), value(new PushInstruction(value)) { };
CurriedCode::CurriedCode(Code *code, PushInstruction *value) : code(code), value(value) { };

size_t CurriedCode::len() { return code->len() + 1; }

Instruction *CurriedCode::at(u32 i)
{
  if (i == 0)
    return value;
  else
    return code->at(i-1);
}

void CurriedCode::set(u32 i, Instruction *is)
{
  code->set(i-1, is);
}

Code *CurriedCode::append(Instruction *ins)
{
  return new CurriedCode(code->append(ins), value);
}
