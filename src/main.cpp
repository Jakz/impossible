/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */


#include "parser/compiler.h"

#include "vm/analysis.h"

#include "vm.h"
#include "instruction.h"
#include "semantics.h"

#include "help.h"

#include <string>
#include <iostream>


#define VAR_DECL1(x, z) z* v1 = (z*)x
#define VAR_DECL2(x, z) z* v2 = (z*)x

using namespace std;

void registerFunctions(MicroCode& mc);

int main (int argc, const char * argv[])
{
  MicroCode mc;
  registerFunctions(mc);
  
  Help::init();

  VM vm(mc);
  
  string input;
  bool finished = false;
  
  /*Code *code = assemble("(?0:[1]1:[1][1<>2<>+])");
  vm->execute(code);
  vm->printTopStack();*/
  
  while (!finished)
  {
    cout << "i>";
    
    getline(cin, input);
    
    
    if (input == "exit")
      finished = true;
    else if (input == "stack")
    {
      cout << " " << vm.stackAsString() << "  <<" << endl;
    }
    else if (input == "wipe")
      vm.wipe();
    else if (input.substr(0,4).compare("help") == 0)
    {
      if (input == "help")
      {
        Help::printHelpMain();
      }
      else
      {
        string params = input.substr(5);
        
        if (params == "operators")
          Help::printOperators();
        else if (params == "types")
          Help::printTypes();
        else if (params == "undocumented")
          Help::printUndocumented();
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
    else if (input == "info")
    {
      cout << "  arch: " << (sizeof(void*) == 4 ? "x86" : "x86-64") << endl;
      cout << "  size of value        : " << sizeof(Value) << (sizeof(Value) == sizeof(value_data) + sizeof(Type) ? "  (packed)" : "") << endl;
      cout << "  size of instruction  : " << sizeof(Instruction) << endl;
      cout << "  size of opcode       : " << sizeof(Opcode) << endl;
    }
    else
    {
      compiler::Compiler compiler;

      
      Code *code = compiler.compile(input);
      
      
      
      if (code)
      {
        vm.execute(code);
        vm.printTopStack();
      }
      
    }
  }
}
