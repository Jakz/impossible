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


#define VAR_DECL1(x, z) z* v1 = (z*)x
#define VAR_DECL2(x, z) z* v2 = (z*)x

using namespace std;

extern void registerFunctions();

int main (int argc, const char * argv[])
{
  registerFunctions();
  Help::init();
  
  VM vm;
  
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
      vm.printStack();
    }
    else if (input.compare("wipe") == 0)
      vm.wipe();
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
        vm.execute(code);
        vm.printTopStack();
      }
      
    }
  }
}

