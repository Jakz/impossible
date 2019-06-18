#ifndef __J80_SCANNER_H__
#define __J80_SCANNER_H__

#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer BaseFlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL compiler::Parser::symbol_type compiler::Lexer::get_next_token()

#define YY_NO_UNISTD_H

#include "parser.hpp"

namespace compiler
{
  class Compiler;
  
  class Lexer : public yyFlexLexer
  {
  public:
    
    Lexer(Compiler &compiler, std::istream *in) : yyFlexLexer(in), compiler(compiler) {}

    virtual compiler::Parser::symbol_type get_next_token();
    virtual ~Lexer() { }
    
  private:
    
    Compiler &compiler;
  };
  
}

#endif
