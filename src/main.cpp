/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */


#include "parser/compiler.h"

#include "vm.h"
#include "instruction.h"

#include "help.h"

#include <string>
#include <iostream>

//#include <curses.h>
/*
class UnaryB {
  const s16 itype;
  public:
    UnaryB(s16 itype) : itype(itype) { }
};

template <Opcode T, class X>
class Unary : public UnaryB
{
public:
  Unary() : UnaryB(X::stype()) { }
  
  Value* ptr(X* x)
  {
    cout << "Missing instruction!" << endl;
    return NULL;
  }
};

template<Opcode T, class X, class Y>
class Binary
{
  public:
    Value* ptr(X* x, Y* y)
    {
      cout << "Missing instruction!" << endl;
    }
};

template<Opcode T, class X, class Y, class Z>
class Ternary
{
public:
  Value* ptr(X* x, Y* y, Z* z)
  {
    cout << "Missing instruction!" << endl;
  }
};


template <>
class Unary<OP_MINUS, Float> : public UnaryB
{
  public:
    Value *ptr(Float *x) { return new Float(-x->get()); };
};

template<>
class Binary<OP_PLUS, Float, Float> {
  public: Value *ptr(Float *x, Float *y) { return new Float(x->get() + y->get()); };
};

template<>
class Binary<OP_PLUS, Int, Int> {
  public: Value *ptr(Int *x, Int *y) { return new Int(x->get() + y->get()); };
};

template<>
class Ternary<OP_RISE, Value, Value, Value> {
  public: Value *ptr(Value *v1, Value *v2, Value *v3) { return NULL; }
};*/


#define VAR_DECL1(x, z) z* v1 = (z*)x
#define VAR_DECL2(x, z) z* v2 = (z*)x

using namespace std;

int main (int argc, const char * argv[])
{
  //Unary<OP_MINUS, Float> unary;
  
  //unary.ptr(new Float(10.0));
  
  //test();
  //return 0;

  
  Help::init();
  
  VM *vm = new VM();
  
  string input;
  bool finished = false;
  
  /*Code *code = assemble("(?0:[1]1:[1][1<>2<>+])");
  vm->execute(code);
  vm->printTopStack();*/
  
  while (!finished)
  {
    cout << "i>";
    
    getline(cin, input);
    
    
    if (input.compare("exit") == 0)
      finished = true;
    else if (input.compare("stack") == 0)
    {
      cout << ">>  ";
      vm->printStack();
    }
    else if (input.compare("wipe") == 0)
      vm->wipe();
    else if (input.substr(0,4).compare("help") == 0)
    {
      if (input.compare("help") == 0)
      {
        Help::printHelpMain();
      }
      else
      {
        string params = input.substr(5);
        
        if (params.compare("operators") == 0)
          Help::printOperators();
        else if (params.compare("types") == 0)
          Help::printTypes();
        else if (Help::printHelpForOperator(params))
        {
          
        }
        else if (Help::printHelpForType(params))
        {
          
        }
        else if (params.length() > 0 && Help::printHelpForSearch(params))
        {
          
        }
      }
    }
    else
    {
      compiler::Compiler compiler;

      
      Code *code = compiler.compile(input);
      
      
      
      if (code)
      {
        vm->execute(code);
        vm->printTopStack();
      }
      
    }
  }
}

