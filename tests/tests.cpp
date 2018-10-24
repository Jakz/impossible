//
//  tests.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#define CATCH_CONFIG_MAIN

#include "compiler.h"

#include "catch.h"

#include "vm.h"

using namespace compiler;

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
  Code* program = Compiler().compile(code.c_str());
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

TEST_CASE("numeric operations")
{
  SECTION("sum")
  {
    executeAndVerifyStack("4 5 +", 9);
  }
}
