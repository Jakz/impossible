#pragma once

#include "lexer.h"
#include "location.hh"
#include "parser.hpp"

#include "../code.h"

namespace compiler
{
  class Compiler
  {
  public:
    void error(const location& l, const std::string& m);
    void error(const std::string& m);

    Code* compile(const std::string& text);
  };
}
