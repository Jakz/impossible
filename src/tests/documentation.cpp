#include "help.h"

void registerFunctions(MicroCode& mc);

struct SOpcode
{
  union {
    u64 value;
    char c[8];
  };

  SOpcode(const char* data) : value(0)
  {
    const size_t length = strlen(data);
    assert(length < 8);
    std::memcpy(&value, data, length);
  }

  bool operator==(const SOpcode& o) const { return value == o.value; }
};


SOpcode operator"" _op(const char* str, size_t length) { return SOpcode(str); }

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