/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#include "Value.h"
#include "instruction.h"
#include "SimpleValue.h"

#include <iostream>
#include <iomanip>
#include <sstream>


const Value* TValue<void*>::NIL = new TValue<void*>(TYPE_NIL);

std::string Value::lvalue()
{
  return svalue() + " : " + type.name();
}


