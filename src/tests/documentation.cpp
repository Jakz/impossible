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


  std::unordered_map<SignatureType, std::vector<OpHelpEntry>, SignatureType::hash> operatorsByType;
  
  const auto& operators = Help::ops();
  for (const auto& entry : operators)
  {
    if (entry.second.i.count() > 0)
      operatorsByType[entry.second.i[0]].push_back(entry.second);
  }

  for (auto& entry : operatorsByType)
  {
    std::sort(entry.second.begin(), entry.second.end(), [](const OpHelpEntry& e1, const OpHelpEntry& e2) { return e1.ident < e2.ident; });

    std::cout << TypeTraits::nameForSignatureType(entry.first) << std::endl;
    for (const auto& op : entry.second)
      std::cout << "  " << op.ident << std::endl;
    std::cout << std::endl;
  }
  

  getchar();

  return 0;
}