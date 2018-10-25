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


namespace math
{
  template<typename T, typename U, typename R> struct plus { public: R operator()(T t, U u) { return t + u;} };
  template<typename T, typename U, typename R> struct minus { public: R operator()(T t, U u) { return t - u;} };
  template<typename T, typename U, typename R> struct times { public: R operator()(T t, U u) { return t * u;} };
  template<typename T, typename U, typename R> struct divide { public: R operator()(T t, U u) { return t / u;} };
  
  template<typename T, typename U, typename R> struct lesser { public: R operator()(T t, U u) { return t < u;} };
  template<typename T, typename U, typename R> struct greater { public: R operator()(T t, U u) { return t > u;} };
  
}

void MicroCode::registerDefault()
{    
    registerNumeric<false, math::plus>(OP_PLUS);
    registerNumeric<false, math::minus>(OP_MINUS);
    registerNumeric<false, math::times>(OP_TIMES);
    registerNumeric<false, math::divide>(OP_DIVIDE);
    
    registerNumeric<true, math::lesser>(OP_LESSER);
    registerNumeric<true, math::greater>(OP_GREATER);
    
    
      
    
    const auto& v = vocabulary();
    
    string_joiner<TypeInfo> argsJoiner("", "", ", ", [] (const auto& t) { return t.name(); }, [] (const auto& t) { return t == TYPE_NONE; });
    
    for (const auto& term : v)
    {
      std::cout << Instruction(term.opcode).svalue() << "  " << argsJoiner.join(term.input.t) << " -> " << argsJoiner.join(term.output.t) << std::endl;
    }
    
    std::cout << "Registered " << vocabulary().size() << " terms." << std::endl;
}
