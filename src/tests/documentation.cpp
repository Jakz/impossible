#include "help.h"

void registerFunctions(MicroCode& mc);

int main(int argc, char* argv[])
{
  MicroCode mc;
  registerFunctions(mc);
  Help::init();
  
  /*
  const auto& operators = Help::ops();
  for (const auto& entry : operators)
  {
    std::cout << entry.first << std::endl;
  }
  */

  getchar();

  return 0;
}