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

#if IMPOSSIBLE_VM_LOG
#define VM_LOG(x) do { std::cout << x << std::endl; } while (false)
#else
#define VM_LOG(x) do { } while (false)
#endif

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

    if (i.isValue())
    {
      VM_LOG("Pushing " << i.svalue());
      push(i.value());
    }
    else
    {
      VM_LOG("Executing " << i.svalue());
      
      if (!microcode.execute(this, i.opcode()))
        i.execute(this);
    }
    
    ++exec.pc;
  }
}

void VM::pushRecord(ActivationRecord&& record)
{
  VM_LOG("Pushing activating record ");
  
  if (exec.code)
    callStack.push(exec);

  exec = record;
}

void VM::popRecord()
{
  if (!callStack.empty())
  {
    VM_LOG("Popping activating record");
    
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
