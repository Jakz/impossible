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

#include <vector>

using namespace compiler;

bool verifyArray(const Array* v1, const std::vector<Value>& v2) { return v1->raw() == v2; }

template<typename... Args> void verifyStack(VM& vm)
{
  REQUIRE(vm.stackSize() == 0);
}

template<typename... Args> void verifyStack(VM& vm, int v, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  Value value = vm.pop();
  REQUIRE(value.type == TYPE_INT);
  REQUIRE(value.integral() == v);
}

template<typename... Args> void verifyStack(VM& vm, real_t v, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  Value value = vm.pop();
  REQUIRE(value.type == TYPE_FLOAT);
  REQUIRE(value.real() == v);
}

template<typename... Args> void verifyStack(VM& vm, bool v, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  Value value = vm.pop();
  REQUIRE(value.type == TYPE_BOOL);
  REQUIRE(value.boolean() == v);
}

template<typename... Args> void verifyStack(VM& vm, const std::vector<Value>& v, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  Value value = vm.pop();
  REQUIRE(value.type == TYPE_ARRAY);
  REQUIRE(verifyArray(value.array(), v));
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
    executeAndVerifyStack("-4", -4);
    executeAndVerifyStack("0", 0);
    executeAndVerifyStack("-0", 0);
    executeAndVerifyStack("3e3", 3000);
  }
  
  SECTION("float")
  {
    executeAndVerifyStack("4.0", 4.0);

  }
}

TEST_CASE("numeric operations")
{
  SECTION("sum")
  {
    executeAndVerifyStack("4 5 +", 9);
  }
}

TEST_CASE("collection constructors")
{
  SECTION("array")
  {
    executeAndVerifyStack("(1 2 3)", std::vector<Value>({1LL,2LL,3LL}));
  }
}

TEST_CASE("collections size")
{
  /* string */
  executeAndVerifyStack("\"\"_", 0);
  executeAndVerifyStack("\"h\"_", 1);
  executeAndVerifyStack("\"he\"_", 2);
  
  /* array */
  executeAndVerifyStack("()_", 0);
  executeAndVerifyStack("(1)_", 1);
  executeAndVerifyStack("(1 2)_", 2);
  
  /* list */
  executeAndVerifyStack("{}_", 0);
  executeAndVerifyStack("{1}_", 1);
  executeAndVerifyStack("{1 2}_", 2);
}


