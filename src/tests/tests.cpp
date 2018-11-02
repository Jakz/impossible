//
//  tests.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include "semantics.h"
extern void registerFunctions(MicroCode& mc);

MicroCode mc;
int main(int argc, char* argv[])
{
  registerFunctions(mc);
  int result = Catch::Session().run(argc, argv);
  return result;
}

#include "compiler.h"
#include "vm.h"

#include <vector>
#include <random>

#define BATCH(x) for (size_t i__ = 0; i__ < (x); ++i__)

using namespace compiler;

bool verifyArray(const Array* v1, const std::vector<Value>& v2) { return v1->raw() == v2; }

template<typename... Args> void verifyStack(VM& vm)
{
  REQUIRE(vm.stackSize() == 0);
}

template<typename... Args> void verifyStack(VM& vm, integral_t v, Args... args)
{
  REQUIRE(vm.stackSize() > 0);
  Value value = vm.pop();
  REQUIRE(value.type == TYPE_INT);
  REQUIRE(value.integral() == v);
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

template<typename... Args> void executeAndVerifyStack(const std::string& code, Args... args)
{
  Code* program = Compiler().compile(code.c_str());
  REQUIRE(program != nullptr);
  
  VM vm(mc);
  vm.execute(program);
  
  verifyStack(vm, args...);
}

template<typename... Args> void executeAndVerifyStack(const std::vector<Instruction>& code, Args... args)
{
  Code* program = new CodeStandard(code);
  REQUIRE(program != nullptr);
  
  VM vm(mc);
  vm.execute(program);
  
  verifyStack(vm, args...);
}

template<typename T> Value dummyData(size_t = 0, size_t = 0) { static_assert(sizeof(T) != sizeof(T), "Must specialize"); }

template<> Value dummyData<String>(size_t min, size_t max)
{
  std::string data = std::string(min + rand()%(max-min+1), ' ');
  for (auto& c : data) { c = 'a' + rand()%26; }
  return new String(data);
}

TEST_CASE("support types")
{
  SECTION("Arguments")
  {
    REQUIRE(SignatureArguments().count() == 0);
    REQUIRE(SignatureArguments(TYPE_INT).count() == 1);
    REQUIRE(SignatureArguments(TYPE_INT, TYPE_INT).count() == 2);
    REQUIRE(SignatureArguments(TYPE_INT, TYPE_INT, TYPE_INT).count() == 3);
  }
}

TEST_CASE("operator< semantics")
{
  SECTION("integral")
  {
    std::mt19937_64 rnd;
    BATCH(1000)
    {
      integral_t i = rnd();
      integral_t j = rnd();
      
      const Value vi = Value(i);
      const Value vj = Value(j);
            
      if (i < j)
      {
        REQUIRE(vi < vj);
        REQUIRE(vi != vj);
        REQUIRE_FALSE(vi > vj);
      }
      else if (i > j)
      {
        REQUIRE(vi > vj);
        REQUIRE(vi != vj);
        REQUIRE_FALSE(vi < vj);
      }
      else
      {
        REQUIRE_FALSE(vi > vj);
        REQUIRE(vi == vj);
        REQUIRE_FALSE(vi < vj);
      }
    }
  }
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

static constexpr size_t REPETITIONS = 100;

TEST_CASE("collections size")
{
  /* string */
  executeAndVerifyStack("\"\"_", 0);
  executeAndVerifyStack("\"h\"_", 1);
  executeAndVerifyStack("\"he\"_", 2);
  
  BATCH(REPETITIONS)
  {
    Value value = dummyData<String>(1, 20);
    REQUIRE(value.string()->size() == value.string()->raw().size());
    executeAndVerifyStack({value, OP_NEG}, value.string()->size());
  }
  
  /* array */
  executeAndVerifyStack("()_", 0);
  executeAndVerifyStack("(1)_", 1);
  executeAndVerifyStack("(1 2)_", 2);
  
  /* list */
  executeAndVerifyStack("{}_", 0);
  executeAndVerifyStack("{1}_", 1);
  executeAndVerifyStack("{1 2}_", 2);
}


