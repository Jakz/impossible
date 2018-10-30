#include "semantics.h"
#include "vm.h"

bool MicroCode::execute(VM* vm, Opcode opcode) const
{
  const size_t stackSize = vm->stackSize();
  
  Value v1 = Value::INVALID, v2 = Value::INVALID, v3 = Value::INVALID;
  
  if (opcodeData[opcode].hasTernary && stackSize >= 3)
  {
    v3 = vm->pop();
    v2 = vm->pop();
    v1 = vm->pop();
    
    auto function = findBestOverload({ opcode, v1.type, v2.type, v3.type });
    
    if (function)
    {
      function->ternary(vm, v1, v2, v3);
      return true;
    }
  }
  
  if (opcodeData[opcode].hasBinary && stackSize >= 2)
  {
    if (v3.valid())
    {
      v2 = v3;
      v1 = v2;
    }
    else
    {
      v2 = vm->pop();
      v1 = vm->pop();
    }
    
    auto function = findBestOverload({ opcode, v1.type, v2.type, TYPE_NONE });
    
    if (function)
    {
      function->binary(vm, v1, v2);
      return true;
    }
  }
  
  if (opcodeData[opcode].hasUnary && stackSize >= 1)
  {
    if (v3.valid())
      v1 = v3;
    else if (v2.valid())
      v1 = v2;
    else
      v1 = vm->pop();
    
    auto function = findBestOverload({ opcode, v1.type, TYPE_NONE, TYPE_NONE });
    
    if (function)
    {
      function->unary(vm, v1);
      return true;
    }
  }
  
  auto function = findBestOverload({ opcode, TYPE_NONE, TYPE_NONE, TYPE_NONE });
  
  if (function)
  {
    function->nullary(vm);
    return true;
  }
  
  
  /* push values back */
  if (v1.valid()) vm->push(v1);
  if (v2.valid()) vm->push(v2);
  if (v3.valid()) vm->push(v3);
  
  
  return false;
}