/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#include "vm.h"

#include "instruction.h"

void VM::execute(Code *code)
{
  running = true;
  
  if (exec.code)
    callStack.push(exec);
  
  exec = ActivationRecord(code);
  
  run();
  
  if (!callStack.empty())
  {
    exec = callStack.top();
    callStack.pop();
  }
}

void VM::run()
{
  while (exec.pc < exec.code->size() && running)
  {
    Instruction *i = exec.code->at(exec.pc);
    
    i->execute(this);
    
    exec.pc++;
  }
}
