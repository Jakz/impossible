#pragma once

#include "lexer.h"
#include "location.hh"
#include "parser.hpp"

class Code;

namespace compiler
{
  class Compiler
  {
  private:
    std::unique_ptr<Code> code;
  public:
    Compiler() { }
    
    void error(const location& l, const std::string& m);
    void error(const std::string& m);

    Code* compile(const std::string& text);
    
    void setCode(const std::vector<Instruction*>& instructions);
  };
}
