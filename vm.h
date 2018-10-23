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
    std::list<Value*>* valueStack;
    std::list<std::list<Value*>*> highStacks;
    std::list<std::list<Value*>*> lowStacks;
    Value *memory[26];
  
    std::stack<ExecEnv> callStack;
    ExecEnv exec;
  
    bool running;
    bool stackPreserve;
    
  public:
  VM() : valueStack(new std::list<Value*>()), exec(ExecEnv(NULL)), running(false), stackPreserve(false), lazy(NULL)
    { 
      for (int i = 0; i < 26; ++i)
        memory[i] = new TValue<void*>(TYPE_NIL);
    }
  
    Code *code()
    {
      return exec.code;
    }
  
    template<typename T> void push(T* object)
    {
      push(new Value(object));
    }
  
    void push(const Value& value)
    {
      valueStack->push_front((Value*)&value);
    }
  
    void push(Value *value)
    {
      valueStack->push_front(value);
    }
  
    void raiseStack()
    {
      std::list<Value*>* next = nullptr;
      
      if (!highStacks.empty())
      {
        next = highStacks.front();
        highStacks.pop_front();
      }
      else
        next = new std::list<Value*>();
      
      lowStacks.push_front(valueStack);
      valueStack = next;
    }
  
    void lowerStack()
    {
      std::list<Value*>* next = nullptr;
      
      if (!lowStacks.empty())
      {
        next = lowStacks.front();
        lowStacks.pop_front();
      }
      else
        next = new std::list<Value*>();
      
      highStacks.push_front(valueStack);
      valueStack = next;
    }
    
    Value* pop()
    {
      if (valueStack->empty())
      {
        stop();
        return nullptr;
      }
      
      Value *v = valueStack->front();
      valueStack->pop_front();
      return v;
    }
  
    Value* pick(int nth)
    {
      //TODO: rotto
      
      if (valueStack->size() == 0)
        return TValue<void*>::NIL->clone();
      
      nth = nth >= valueStack->size() ? (int)valueStack->size() - 1 : nth;
      
      std::list<Value*>::iterator it = valueStack->begin();
      advance(it, nth);
      
      return (*it)->clone();
    }
  
    Value* peek()
    {
      if (valueStack->empty())
      {
        stop();
        return NULL;
      }
      
      return valueStack->front();
    }
  
    size_t stackSize() const { return valueStack->size(); }
  
    bool stackHasValues(int count) const{ return valueStack->size() >= count; }
  
    bool popThree(Value ** v1, Value ** v2, Value ** v3)
    {
      *v3 = pop();
      
      if (running)
        *v2 = pop();
      
      if (running)
        *v1 = pop();
      
      if (!running)
        std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc-1)->svalue() << "\' required two values." << std::endl;

      return running;
    }  
  
    bool popTwo(Value ** v1, Value ** v2)
    {
      *v2 = pop();
      
      if (running)
        *v1 = pop();
      
      if (!running)
        std::cout << "ERROR: stack was empty but instruction \'" << exec.code->at(exec.pc-1)->svalue() << "\' required two values." << std::endl;

      return running;
    }
  
    bool popOne(Value **v1)
    {
      *v1 = pop();
      
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
      std::list<Value*>::const_iterator it;
      
      for (it = valueStack->begin(); it != valueStack->end(); ++it)
      {
        std::cout << (*it)->svalue();
        
        if (*it != valueStack->back())
          std::cout << " ";
      }
      
      std::cout << std::endl;
    }
  
    void printTopStack() const
    {
      if (!valueStack->empty())
      {
        std::cout << "  " << valueStack->front()->lvalue() << std::endl;
      }
    }
  
    void stackMode(bool preserve)
    {
      stackPreserve = preserve;
    }
  
    LazyArrayHolder *lazy;
};

#endif
