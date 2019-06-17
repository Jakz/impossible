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
    ss << at(i).svalue();
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
    ss << at(i).svalue() << " ";
    if (i+1 == pc)
      l = ss.str().length();
    
  }
  
  ss << std::endl;
  ss << std::string(l, ' ');
  ss << '^';
  
  
  return ss.str();
}
