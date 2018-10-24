/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#ifndef _VM_H_
#define _VM_H_

#include <iostream>
#include <list>
#include <stack>
#include <array>

#include "Values.h"
#include "instruction.h"

class Instruction;

struct ExecEnv
{
  Code *code;
  u32 pc;
  
  ExecEnv(Code *code) : code(code), pc(0) { }
  
  void set(Code *code) { this->code = code; pc = 0; }
};

class VM
{
private:
  using stack_t = std::vector<Value>;
  
  stack_t* valueStack;
  std::list<stack_t*> highStacks;
  std::list<stack_t*> lowStacks;
  std::array<Value, 26> memory;
  
  std::stack<ExecEnv> callStack;
  ExecEnv exec;
  
  bool running;
  bool stackPreserve;
  
public:
  VM() : valueStack(new stack_t()), exec(ExecEnv(NULL)), running(false), stackPreserve(false), lazy(NULL), memory()
  {
  }
  
  Code *code()
  {
    return exec.code;
  }
  
  template<typename T> void push(T* object)
  {
    push(Value(object));
  }
  
  void push(const Value& value)
  {
    valueStack->push_back(value);
  }

  void raiseStack()
  {
    stack_t* next = nullptr;
    
    if (!highStacks.empty())
    {
      next = highStacks.front();
      highStacks.pop_front();
    }
    else
      next = new stack_t();
    
    lowStacks.push_front(valueStack);
    valueStack = next;
  }
  
  void lowerStack()
  {
    stack_t* next = nullptr;
    
    if (!lowStacks.empty())
    {
      next = lowStacks.front();
      lowStacks.pop_front();
    }
    else
      next = new stack_t();
    
    highStacks.push_front(valueStack);
    valueStack = next;
  }
  
  Value pop()
  {
    if (valueStack->empty())
    {
      stop();
      return Value(TYPE_INVALID);
    }
    
    Value v = valueStack->back();
    valueStack->pop_back();
    return v;
  }
  
  Value pick(integral_t nth)
  {
    //TODO: rotto
    
    if (valueStack->size() == 0)
      return Value(TYPE_INVALID);
    
    nth = nth >= valueStack->size() ? valueStack->size() - 1 : nth;
    
    return valueStack->at(valueStack->size() - nth);
  }
  
  Value peek()
  {
    if (valueStack->empty())
    {
      stop();
      return Value(TYPE_INVALID);
    }
    
    return valueStack->front();
  }
  
  size_t stackSize() const { return valueStack->size(); }
  
  bool stackHasValues(int count) const{ return valueStack->size() >= count; }
  
  bool popThree(Value& v1, Value& v2, Value& v3)
  {
    v3 = pop();
    
    if (running)
      v2 = pop();
    
    if (running)
      v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc-1)->svalue() << "\' required two values." << std::endl;
    
    return running;
  }
  
  bool popTwo(Value& v1, Value& v2)
  {
    v2 = pop();
    
    if (running)
      v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc-1)->svalue() << "\' required two values." << std::endl;
    
    return running;
  }
  
  bool popOne(Value& v1)
  {
    v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc-1)->svalue() << "\' required a value." << std::endl;
    
    return running;
  }
  
  void wipe()
  {
    std::list<Value*>::iterator it;
    
    //for (it = valueStack.begin(); it != valueStack.end(); ++it)
    //  delete *it;
    
    valueStack->clear();
  }
  
  void execute(Code *code);
  
  void run();
  
  void stop()
  {
    running = false;
  }
  
  void printStack() const
  {
    stack_t::const_iterator it;
    
    bool first = true;
    for (const auto& value : *valueStack)
    {
      if (first)
        first = false;
      else
        std::cout << " ";
        
      std::cout << value.svalue();
      
    }
    
    std::cout << std::endl;
  }
  
  void printTopStack() const
  {
    if (!valueStack->empty())
    {
      std::cout << "  " << valueStack->front().lvalue() << std::endl;
    }
  }
  
  void stackMode(bool preserve)
  {
    stackPreserve = preserve;
  }
  
  LazyArrayHolder *lazy;
};

#endif
