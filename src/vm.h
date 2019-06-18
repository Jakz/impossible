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

#include "value.h"
#include "collection.h"

#include "instruction.h"

#include "types/error.h"

class Instruction;
class MicroCode;
class LazyArrayHolder;

struct ActivationRecord
{
  Code* code;
  const LazyArrayHolder* lazy;
  size_t pc;

  ActivationRecord() : code(nullptr), lazy(nullptr), pc(0) { }
  ActivationRecord(Code *code) : code(code), lazy(nullptr), pc(0) { }
  ActivationRecord(const LazyArrayHolder* lazy) : code(nullptr), lazy(lazy), pc(0) { }

  operator bool() const { return lazy || code; }
  
  //void set(Code *code) { this->code = code; pc = 0; }
};

class VM
{
private:
  using stack_t = std::vector<Value>;
  
  stack_t* valueStack;
  std::list<stack_t*> highStacks;
  std::list<stack_t*> lowStacks;
  std::array<Value, 26> memory;
  
  std::stack<ActivationRecord> callStack;
  ActivationRecord exec;
  
  bool running;
  bool stackPreserve;
  
  const MicroCode& microcode;
  
public:
  VM(MicroCode& microcode) : valueStack(new stack_t()), exec(ActivationRecord()), running(false), stackPreserve(false), memory(),
  microcode(microcode)
  {
  }
  
  Code *code()
  {
    return exec.code;
  }

  void pushRecord(ActivationRecord&& record);
  void popRecord();

  const auto& record() const { return exec; }
  
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
      IMPOSSIBLE_THROW(OperandRequiredOnStackException());
      stop();
      return Value::INVALID;
    }
    
    Value v = valueStack->back();
    valueStack->pop_back();
    return v;
  }
  
  Value pick(integral_t nth);
  
  Value peek()
  {
    if (valueStack->empty())
    {
      stop();
      return Value::INVALID;
    }
    
    return valueStack->front();
  }
  
  size_t stackSize() const { return valueStack->size(); }
    
  bool popThree(Value& v1, Value& v2, Value& v3)
  {
    v3 = pop();
    
    if (running)
      v2 = pop();
    
    if (running)
      v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc).svalue() << "\' required three values." << std::endl;
    
    return running;
  }
  
  bool popTwo(Value& v1, Value& v2)
  {
    v2 = pop();
    
    if (running)
      v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc).svalue() << "\' required two values." << std::endl;
    
    return running;
  }
  
  bool popOne(Value& v1)
  {
    v1 = pop();
    
    if (!running)
      std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc).svalue() << "\' required a value." << std::endl;
    
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
  
  std::string stackAsString() const;
  
  void printTopStack() const
  {
    if (!valueStack->empty())
    {
      const Value& value = valueStack->back();
      std::cout << "  " << value.svalue() + " : " + value.type.name() << std::endl;
    }
  }
  
  void stackMode(bool preserve)
  {
    stackPreserve = preserve;
  }
};

#endif
