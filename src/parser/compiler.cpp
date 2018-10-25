#include "compiler.h"

#include <sstream>

using namespace compiler;
using namespace std;

void Compiler::error(const location& l, const std::string& m)
{
  cerr << "Compiler error at " /* << file << ": "*/ << l.begin.line << "," << l.begin.column << " : " << m << endl;
}

void Compiler::error(const std::string& m)
{
  cerr << "Compiler error: " << m << endl;
}

Code* Compiler::compile(const std::string &text)
{
  bool shouldGenerateTrace = false;
  
  istringstream ss(text);
  
  compiler::Lexer lexer(*this, &ss);
  compiler::Parser parser(lexer, *this);
  parser.set_debug_level(shouldGenerateTrace);
  
  return parser.parse() == 0 ? code.release() : nullptr;
}

void Compiler::setCode(const std::vector<Instruction>& instructions)
{
  code.reset(new CodeStandard(instructions));
}
