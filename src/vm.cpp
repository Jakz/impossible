/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#include "vm.h"

#include "semantics.h"
#include "instruction.h"

#include "types/error.h"

const MicroCode& defaultCode()
{
  static MicroCode code;
  return code;
}

void VM::execute(Code* code)
{
  running = true;
  
  pushRecord(ActivationRecord(code));
  run();
  popRecord();
}

void VM::run()
{
  while (exec.pc < exec.code->size() && running)
  {
    const Instruction& i = exec.code->at(exec.pc);

    std::cout << "Executing " << i.svalue() << std::endl;
    
    if (i.isValue())
      push(i.value());
    else
    {
      if (!microcode.execute(this, i.opcode()))
        i.execute(this);
    }
    
    ++exec.pc;
  }
}

void VM::pushRecord(ActivationRecord&& record)
{
  if (exec.code)
    callStack.push(exec);

  exec = record;
}

void VM::popRecord()
{
  if (!callStack.empty())
  {
    exec = callStack.top();
    callStack.pop();
  }
}

Value VM::pick(integral_t nth)
{
  if (nth + 1 > valueStack->size())
    return new Error(ErrorCode::OPERAND_REQUIRED_ON_STACK, "pick index request out of bounds");
  //nth = std::min(nth, (integral_t)valueStack->size() - 1);
  else
    return *(valueStack->end() - (nth + 1));
}

std::string VM::stackAsString() const
{
  string_joiner<Value> joiner("", "", ", ", [] (const Value& value) { return value.svalue(); });
  return joiner.join(*valueStack);
}
