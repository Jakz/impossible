//
//  tests.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#define CATCH_CONFIG_MAIN
#include "catch.h"

#include "vm.h"

template<typename... Args> void verifyStack(VM& vm)
{
  REQUIRE(vm.stackSize() == 0);
}

template<typename... Args> void verifyStack(VM& vm, integral_t i, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  auto value = vm.pop();
  REQUIRE(value->type == TYPE_INT);
  REQUIRE(value->integral() == i);
}

template<typename... Args> void executeAndVerifyStack(std::string code, Args... args)
{
  Code* program = assemble(code.c_str());
  REQUIRE(program != nullptr);
  
  VM vm;
  vm.execute(program);
  
  verifyStack(vm, args...);
}

TEST_CASE("primitive literals")
{
  SECTION("int")
  {
    executeAndVerifyStack("4", 4);
    executeAndVerifyStack("10k", 10000);
    executeAndVerifyStack("2kk", 2000000);
  }
  
  
  
}
