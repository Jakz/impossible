/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#include "help.h"

#include <iostream>

using namespace std;

std::multimap<string, OpHelpEntry> Help::operators;
std::multimap<Type, TypeConstructor> Help::constructors;


const char* Help::topicString(Topic topic)
{
  switch (topic)
  {
    case Topic::NUMERICS: return "numeric";
    case Topic::BITWISE: return "bitwise";
    case Topic::STACK: return "stack";
    case Topic::COLLECTIONS: return "collections";
    case Topic::UTILITY: return "utility";
    case Topic::FUNCTIONAL: return "functional";
    case Topic::LOGIC: return "logic";
    case Topic::TEXT: return "text";
  }
}

#include "instruction.h"
void Help::addOperator(Opcode opcode, const SignatureArguments& input, const SignatureArguments& output, Topic topic, const std::string& name, const std::string& description, const OpHelpEntry::examples_t& examples)
{
  addOperator(Instruction(opcode).svalue(),
              OpHelpEntry(name, topic, input, output, description, examples)
              );
}

void Help::init()
{
  //return;
  
  addOperator("+", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "sum", Topic::NUMERICS, "addition between integers", "2 2 +", "4"));
  addOperator("+", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "sum", Topic::NUMERICS, "addition between floats", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_CHAR, TYPE_INT, TYPE_CHAR, "shift", Topic::TEXT, "shift a char by adding an integer value to its value", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_CHAR, TYPE_CHAR, TYPE_STRING, "concatenate", Topic::TEXT, "concatenate two strings", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_STRING, TYPE_GENERIC, TYPE_STRING, "append", Topic::TEXT, "append string representation of value to string", "", ""));
  
  addOperator("+:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic sum", Topic::COLLECTIONS, "applies + operation between each pair of elements in lists", "", ""));
  addOperator("+:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic sum", Topic::COLLECTIONS, "applies + operation between each pair of elements in arrays", "", ""));
  
  addOperator("+.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic sum", Topic::COLLECTIONS, "applies + operation between each pair of elements in the list", "", ""));
  addOperator("+.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic sum", Topic::COLLECTIONS, "applies + operation between each pair of elements in the array", "", ""));
  
  addOperator("-:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic sub", Topic::COLLECTIONS, "applies - operation between each pair of elements in lists", "", ""));
  addOperator("-:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic sub", Topic::COLLECTIONS, "applies - operation between each pair of elements in arrays", "", ""));
  
  addOperator("-.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic sub", Topic::COLLECTIONS, "applies - operation between each pair of elements in the list", "", ""));
  addOperator("-.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic sub", Topic::COLLECTIONS, "applies - operation between each pair of elements in the array", "", ""));
  
  addOperator("*:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic times", Topic::COLLECTIONS, "applies * operation between each pair of elements in lists", "", ""));
  addOperator("*:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic times", Topic::COLLECTIONS, "applies * operation between each pair of elements in arrays", "", ""));
  
  addOperator("*.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic times", Topic::COLLECTIONS, "applies * operation between each pair of elements in the list", "", ""));
  addOperator("*.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic times", Topic::COLLECTIONS, "applies * operation between each pair of elements in the array", "", ""));
  
  addOperator("/:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic div", Topic::COLLECTIONS, "applies / operation between each pair of elements in lists", "", ""));
  addOperator("/:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic div", Topic::COLLECTIONS, "applies / operation between each pair of elements in arrays", "", ""));
  
  addOperator("/.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic div", Topic::COLLECTIONS, "applies / operation between each pair of elements in the list", "", ""));
  addOperator("/.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic div", Topic::COLLECTIONS, "applies / operation between each pair of elements in the array", "", ""));
  
  addOperator("-", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "sub", Topic::NUMERICS, "subtraction between integers", "", ""));
  addOperator("-", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "sub", Topic::NUMERICS, "subtraction between floats", "", ""));
  addOperator("-", OpHelpEntry::binary(TYPE_CHAR, TYPE_INT, TYPE_CHAR, "shift", Topic::TEXT, "shift a char by subtracting an integer value to its value", "", ""));
  
  addOperator("/", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "div", Topic::NUMERICS, "division between integers", "", ""));
  addOperator("/", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "div", Topic::NUMERICS, "division between floats", "", ""));
  addOperator("/", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "difference", Topic::COLLECTIONS, "computes set difference", "", ""));
  
  addOperator("*", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "mul", Topic::NUMERICS, "multiplication between integers", "", ""));
  addOperator("*", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "mul", Topic::NUMERICS, "multiplication between floats", "", ""));
  
  addOperator("_", OpHelpEntry::unary(TYPE_INT, TYPE_INT, "negation", Topic::NUMERICS, "negates integer", "", ""));
  addOperator("_", OpHelpEntry::unary(TYPE_FLOAT, TYPE_FLOAT, "negation", Topic::NUMERICS, "negates float", "", ""));
  
  addOperator("//", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "mod", Topic::NUMERICS, "computes modulo between integers", "", ""));
  addOperator("//", OpHelpEntry::unaryO(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "modf", Topic::NUMERICS, "computes integer and fractional part of a float number", "", ""));
  
  addOperator("<", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_BOOL, "lesser", Topic::NUMERICS, "pushes true on stack if first value is lesser than second", "", ""));
  addOperator("<", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_BOOL, "lesser", Topic::NUMERICS, "pushes true on stack if first value is lesser than second", "", ""));
  addOperator("<", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "min", Topic::COLLECTIONS, "finds minimum element of a list", "", ""));
  addOperator("<", OpHelpEntry::unaryO(TYPE_ARRAY, TYPE_ARRAY, TYPE_GENERIC, "min", Topic::COLLECTIONS, "finds minimum element of an array", "", ""));

  
  addOperator(">", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_BOOL, "greater", Topic::NUMERICS, "pushes true on stack if first value is greater than second", "", ""));
  addOperator(">", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_BOOL, "greater", Topic::NUMERICS, "pushes true on stack if first value is greater than second", "", ""));
  addOperator(">", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "max", Topic::COLLECTIONS, "finds maximum element of a list", "", ""));
  addOperator(">", OpHelpEntry::unaryO(TYPE_ARRAY, TYPE_ARRAY, TYPE_GENERIC, "max", Topic::COLLECTIONS, "finds maximum element of an array", "", ""));
  
  addOperator("&", OpHelpEntry::binary(TYPE_BOOL, TYPE_BOOL, TYPE_BOOL, "and", Topic::LOGIC, "computes logical and between two booleans", "", ""));
  addOperator("&", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "and", Topic::BITWISE, "computes bitwise and between two integers", "", ""));
  addOperator("&", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "intersection", Topic::COLLECTIONS, "computes set intersection", "", ""));
  
  addOperator("|", OpHelpEntry::binary(TYPE_BOOL, TYPE_BOOL, TYPE_BOOL, "or", Topic::LOGIC, "computes logical or between two booleans", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "or", Topic::BITWISE, "computes bitwise or between two integers", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "union", Topic::COLLECTIONS, "computes set union", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_RANGE, TYPE_RANGE, TYPE_RANGE, "union", Topic::NUMERICS, "computes union of ranges", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_RANGE, TYPE_INT, TYPE_RANGE, "union", Topic::NUMERICS, "insert range composed by single integer to range", "", ""));
  
  addOperator("~", OpHelpEntry::unary(TYPE_BOOL, TYPE_BOOL, "not", Topic::LOGIC, "computes logical not of a boolean", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_INT, TYPE_INT, "not", Topic::BITWISE, "computes bitwise not of an integer", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_FLOAT, TYPE_FLOAT, "reciprocal", Topic::NUMERICS, "computes reciprocal (1/n) of float value", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "reverse", Topic::COLLECTIONS, "reverses a list", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "reverse", Topic::COLLECTIONS, "reverses an array", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_STRING, TYPE_STRING, "reverse", Topic::TEXT, "reverses a string", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_RANGE, TYPE_ARRAY, "concretize", Topic::COLLECTIONS, "build an explicit integer array from a range", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_LAZY_ARRAY, TYPE_ARRAY, "concretize", Topic::COLLECTIONS, "build a concrete array with all elements already generated in a lazy array", "", ""));
  
  addOperator(">>", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "right shift", Topic::BITWISE, "integer right shift of specified amount", "", ""));
  addOperator(">>", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "filter", Topic::COLLECTIONS, "filter list by keeping only values that satisfy predicate", "", ""));
  addOperator(">>", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "filter", Topic::COLLECTIONS, "filter array by keeping only values that satisfy predicate", "", ""));
  
  addOperator("<<", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "left shift", Topic::BITWISE, "integer left shift of specified amount", "", ""));
  
  addOperator("!", OpHelpEntry::unary(TYPE_LAMBDA, TYPE_UNKNOWN, "apply", Topic::FUNCTIONAL, "pop first value from stack and executes it as a lambda", "1 [2+] !", "3"));
  addOperator("!", OpHelpEntry::unary(TYPE_INT, TYPE_FLOAT, "factorial", Topic::NUMERICS, "computes factorial", "", ""));
  addOperator("=", OpHelpEntry::binary(TYPE_GENERIC, TYPE_GENERIC, TYPE_BOOL, "eq", Topic::LOGIC, "push true on stack if two topmost values are equal", "", ""));
  
  addOperator("^", OpHelpEntry::unary(TYPE_GENERIC, "peek", Topic::UTILITY, "prints stack topmost value", "", ""));
  
   addOperator("s", OpHelpEntry::unary(TYPE_GENERIC, TYPE_STRING, "string cast", Topic::TEXT, "convert the value to string", "", ""));
  
  addOperator("?", OpHelpEntry::binary(TYPE_BOOL, TYPE_LAMBDA, TYPE_UNKNOWN, "if", Topic::LOGIC, "executes lambda if boolean value is true", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if value is contained in list", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_GENERIC, TYPE_ARRAY, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if value is contained in array", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_STACK, TYPE_GENERIC, TYPE_STACK, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if value is contained in stack", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_QUEUE, TYPE_GENERIC, TYPE_QUEUE, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if value is contained in queue", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_SET, TYPE_GENERIC, TYPE_SET, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if value is contained in set", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_MAP, TYPE_GENERIC, TYPE_MAP, TYPE_BOOL, "contains", Topic::COLLECTIONS, "pushes true if key is contained in map", "", ""));
  
  
  addOperator("??", OpHelpEntry::ternary(TYPE_BOOL, TYPE_LAMBDA, TYPE_LAMBDA, TYPE_UNKNOWN, "if-else", Topic::LOGIC, "executes first lambda if boolean is true, second lambda otherwise", "", ""));
  
  addOperator("@", OpHelpEntry::unaryO(TYPE_STACK, TYPE_STACK, TYPE_GENERIC, "pop", Topic::COLLECTIONS, "pop element from stack", "", ""));
  addOperator("@", OpHelpEntry::unaryO(TYPE_QUEUE, TYPE_QUEUE, TYPE_GENERIC, "pop", Topic::COLLECTIONS, "pop element from top of the queue", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_MAP, TYPE_GENERIC, TYPE_MAP, TYPE_GENERIC2, "get", Topic::COLLECTIONS, "get element specified by key from map, pushes nil if key is not present", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_LAZY_ARRAY, TYPE_INT, TYPE_LAZY_ARRAY, TYPE_GENERIC, "at", Topic::COLLECTIONS, "retrieves n-th element from the lazy array, value will be generated if needed", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_LAZY_ARRAY, TYPE_RANGE, TYPE_LAZY_ARRAY, TYPE_ARRAY, "extract", Topic::COLLECTIONS, "retrieves all the elements specified by the range from the lazy array", "", ""));
  

  addOperator("#", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_INT, TYPE_GENERIC, TYPE_ARRAY, "set", Topic::COLLECTIONS, "set n-th element specified by integer of array", "", ""));
  addOperator("#", OpHelpEntry::ternary(TYPE_MAP, TYPE_GENERIC, TYPE_GENERIC2, TYPE_MAP, "set", Topic::COLLECTIONS, "set value for key in map", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_SET, TYPE_GENERIC, TYPE_SET, "insert", Topic::COLLECTIONS, "insert element in set", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_STACK, TYPE_GENERIC, TYPE_STACK, "push", Topic::COLLECTIONS, "push an element on top of the stack", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_QUEUE, TYPE_GENERIC, TYPE_QUEUE, "push", Topic::COLLECTIONS, "push element back in the queue", "", ""));
  
  addOperator("<#", OpHelpEntry::binary(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, "push front", Topic::COLLECTIONS, "push value on top of the list", "", ""));
  addOperator(">#", OpHelpEntry::binary(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, "push back", Topic::COLLECTIONS, "push value on back of the list", "", ""));
  addOperator("<@", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "pop front", Topic::COLLECTIONS, "pop value from top of the list", "", ""));
  addOperator(">@", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "pop back", Topic::COLLECTIONS, "pop value from back of the list", "", ""));

  addOperator(".>", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each element of the list", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each element of the array", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each element of the set", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_STRING, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each character of the string", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_MAP, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each value of the map (keys are not pushed)", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_RANGE, TYPE_LAMBDA, TYPE_UNKNOWN, "each", Topic::COLLECTIONS, "executes the lambda for each integer contained in range", "", ""));
  
  addOperator(".>:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_UNKNOWN, "each with index", Topic::COLLECTIONS, "executes the lambda for each element of the array by pushing also the index", "", ""));
  addOperator(".>:", OpHelpEntry::binary(TYPE_MAP, TYPE_LAMBDA, TYPE_UNKNOWN, "each with key", Topic::COLLECTIONS, "executes the lambda for each pair of the map by pushing both key and value", "", ""));

  addOperator(".>.", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "map", Topic::COLLECTIONS, "applies lambda to each element of the list to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "map", Topic::COLLECTIONS, "applies lambda to each element of the array to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_RANGE, TYPE_LAMBDA, TYPE_ARRAY, "map", Topic::COLLECTIONS, "applies lambda to each element of the range to build an array", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_STRING, TYPE_LAMBDA, TYPE_STRING, "map", Topic::COLLECTIONS, "applies lambda to each character of the string to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_SET, "map", Topic::COLLECTIONS, "applies lambda to each element of the set to build a new one", "", ""));

  
  addOperator(":>:", OpHelpEntry::ternary(TYPE_LIST, TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "dmap", Topic::COLLECTIONS, "applies lambda to each pair of elements from both lists to build a new one", "", ""));
  addOperator(":>:", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "dmap", Topic::COLLECTIONS, "applies lambda to each pair of elements from both arrays to build a new one", "", ""));
  
  addOperator(":*.", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "cartesian", Topic::COLLECTIONS, "applies lambda to each possible pair of elements from both arrays to build a new one", "", ""));
  addOperator(":*.", OpHelpEntry::ternary(TYPE_RANGE, TYPE_RANGE, TYPE_LAMBDA, TYPE_ARRAY, "cartesian", Topic::COLLECTIONS, "applies lambda to each possible pair of elements from both ranges to build a new one", "", ""));


  addOperator(":>.", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", Topic::COLLECTIONS, "folds an array by taking accumulator value and passing it through each element", "", ""));
  addOperator(":>.", OpHelpEntry::ternary(TYPE_LIST, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", Topic::COLLECTIONS, "folds a list by taking accumulator value and passing it through each element", "", ""));
  addOperator(":>.", OpHelpEntry::ternary(TYPE_SET, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", Topic::COLLECTIONS, "folds a set by taking accumulator value and passing it through each element", "", ""));

  
  addOperator(".?", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "min", Topic::NUMERICS, "pushes the minimum of two integer numbers", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "min", Topic::NUMERICS, "pushes the minimum of two float numbers", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_BOOL, "any", Topic::COLLECTIONS, "pushes true on stack if at least one element in the list satisfies the lambda", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_BOOL, "any", Topic::COLLECTIONS, "pushes true on stack if at least one element in the array satisfies the lambda", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_BOOL, "any", Topic::COLLECTIONS, "pushes true on stack if at least one element in the set satisfies the lambda", "", ""));

  addOperator(":?", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "max", Topic::NUMERICS, "pushes the maximum of two integer numbers", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "max", Topic::NUMERICS, "pushes the maximum of two float numbers", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_BOOL, "every", Topic::COLLECTIONS, "pushes true on stack if all the elements in the list satisfies the lambda", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_BOOL, "every", Topic::COLLECTIONS, "pushes true on stack if all the elements in the array satisfies the lambda", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_BOOL, "every", Topic::COLLECTIONS, "pushes true on stack if all the elements in the set satisfies the lambda", "", ""));
  
  addOperator("@>", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "rand", Topic::NUMERICS, "generate a random value between two pushed values (not inclusive of upper bound)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_RANGE, TYPE_INT, "rand", Topic::NUMERICS, "extracts a random integer from a range (with uniform probability between all values)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_BOOL, TYPE_BOOL, "rand flag", Topic::LOGIC, "generate a random truth value (true or false)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "rand get", Topic::COLLECTIONS, "extracts a random value from an array", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "rand get", Topic::COLLECTIONS, "extracts a random value from a list", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_SET, TYPE_GENERIC, "rand get", Topic::COLLECTIONS, "extracts a random value from a set", "", ""));
  
  addOperator("#>", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "sort", Topic::COLLECTIONS, "sorts the specified list", "", ""));
  addOperator("#>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "sort", Topic::COLLECTIONS, "sorts the specified array", "", ""));

  addOperator("$>", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "shuffle", Topic::COLLECTIONS, "shuffles the specified list", "", ""));
  addOperator("$>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "shuffle", Topic::COLLECTIONS, "shuffles the specified array", "", ""));  
  

  addOperator("[#]", OpHelpEntry::nullaryO(TYPE_LAMBDA, "recur", Topic::FUNCTIONAL, "pushes lambda in execution on stack", "", ""));
  
  addOperator("<>", OpHelpEntry::binary(TYPE_LAMBDA, TYPE_LAMBDA, TYPE_UNKNOWN, "while", Topic::FUNCTIONAL, "executes second lambda while first lambda evaluates to true", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_INT, TYPE_LAMBDA, TYPE_UNKNOWN, "counter", Topic::FUNCTIONAL, "executes second lambda a number of times specified by integer", "", ""));
  addOperator("<>", OpHelpEntry::binaryO(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, TYPE_LIST, "partition", Topic::COLLECTIONS, "partition a list according to predicate given by lambda", "", ""));
  addOperator("<>", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, TYPE_ARRAY, "partition", Topic::COLLECTIONS, "partition an array according to predicate given by lambda", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_LAZY_ARRAY, TYPE_LAMBDA, TYPE_LAZY_ARRAY, "generate", Topic::COLLECTIONS, "generates terms from a lazy array until predicate over the value becomes false", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_LAZY_ARRAY, TYPE_INT, TYPE_LAZY_ARRAY, "generate", Topic::COLLECTIONS, "generates terms until size of the lazy array is equal to specified integer", "", ""));
  addOperator("<>", OpHelpEntry::unary(TYPE_INT, TYPE_GENERIC, "inner get", Topic::FUNCTIONAL, "(ONLY AVAILABLE INSIDE A LAZY LIST DECLARATION) fetches a previous element from the lazy list", "", ""));

  
  
  addConstructor(TYPE_INT, TypeConstructor::make("10", "an decimal integer literal"));
  addConstructor(TYPE_INT, TypeConstructor::make("xFF", "a hexadecimal integer literal"));
  addConstructor(TYPE_INT, TypeConstructor::make("b1011", "a binary integer literal"));
  addConstructor(TYPE_INT, TypeConstructor::make("3k", "short hand to multiply by 1000"));
  addConstructor(TYPE_INT, TypeConstructor::make("2kk", "short hand to multiply by 1000000"));

  
  addConstructor(TYPE_BOOL, TypeConstructor::make("t", "true value"));
  addConstructor(TYPE_BOOL, TypeConstructor::make("f", "false value"));

  addConstructor(TYPE_LIST, TypeConstructor::make("{ 'a 'b (1 2) 2.23}", "creates a list with specified elements"));
  addConstructor(TYPE_LIST, TypeConstructor::make("{}", "creates an empty list"));
  
  addConstructor(TYPE_ARRAY, TypeConstructor::make("( 1 'a {1 2 3})", "creates an array with specified elements"));
  addConstructor(TYPE_ARRAY, TypeConstructor::make("(:10)", "creates an array with specified size and all elements initialized to nil"));
  addConstructor(TYPE_ARRAY, TypeConstructor::make("(:5 'a)", "creates an array with specified size and all elements initialized to value specified"));
  addConstructor(TYPE_ARRAY, TypeConstructor::make("()", "creates an empty array, mind that this is not efficient because it needs resizing"));
  
  addConstructor(TYPE_LAZY_ARRAY, TypeConstructor::make("(?i[2*])", "creates a lazy array whose elements are defined by a lambda which takes the index as input"));
  addConstructor(TYPE_LAZY_ARRAY, TypeConstructor::make("(?0:[1]1:[1][2*])", "creates a lazy array with a generic lambda and some predefined lambdas for specific indices"));
  
  addConstructor(TYPE_SET, TypeConstructor::make("{. 1 2 3 'a 'b}", "creates a set with specified elements (duplicates will be discarded)"));
  addConstructor(TYPE_SET, TypeConstructor::make("{.}", "creates an empty set"));
  
  addConstructor(TYPE_STACK, TypeConstructor::make("{> 1 2 3 'a 'b}", "creates a stack with specified elements"));
  addConstructor(TYPE_STACK, TypeConstructor::make("{>}", "creates an empty stack"));
  
  addConstructor(TYPE_QUEUE, TypeConstructor::make("{< 1 2 3 'a 'b}", "creates a queue with specified elements"));
  addConstructor(TYPE_QUEUE, TypeConstructor::make("{<}", "creates an empty queue"));
  
  addConstructor(TYPE_MAP, TypeConstructor::make("{'a:1 2:\"foo\"}", "creates a map with specified pairs"));
  addConstructor(TYPE_MAP, TypeConstructor::make("{:}", "creates an empty map"));
  
  cout << "Impossible++ v0.3-r50" << endl;
  printHelpSummary();
}

void Help::printHelpMain()
{
  cout << "Impssible++ executes code that is entered directly in this toplevel interpreter." << endl;
  cout << "Available top level commands:" << endl;
  cout << "  help  : shows this screen" << endl;
  cout << "  help types      : shows types that are supported by the language" << endl;
  cout << "  help operators  : shows available operators on types" << endl;
  cout << "  help 'type'     : shows all operators available for type specified" << endl;
  cout << "  help 'operator' : shows all instructions for specified operator" << endl;
  cout << "  help 'term'     : search for term inside name or desc of available instruction" << endl;
  cout << "  stack : prints the content of the stack" << endl;
  cout << "  wipe  : empties the stack" << endl;
  cout << "  info  : displays information about environment" << endl;
  cout << "  exit  : exits the interpreter" << endl;
}

void Help::printHelpSummary()
{
  cout << "  - documented instructions: " << operators.size() << endl;
  printOperators();
  printTypes();
  cout << "  - type 'help' to get additional informations" << endl;
}

void Help::addConstructor(Type type, TypeConstructor constructor)
{
  constructors.emplace(std::make_pair(type, constructor));
}

void Help::addOperator(string op, OpHelpEntry entry)
{
  operators.insert(std::make_pair(op, entry));
}

void Help::printOperator(string op, const OpHelpEntry& o)
{
  cout << "  " << op << " (" << o.ident << ") : ";
  
  const size_t oc = o.o.count(), ic = o.i.count();
  
  for (int i = 0; i < ic; ++i)
  {
    cout << TypeTraits::nameForSignatureType(o.i[i]);
    
    if (i < ic - 1)
      cout << ", ";
    else if (oc > 0)
      cout << " -> ";
  }
  
  if (ic == 0 || oc == 0)
    cout << " -> ";
  
  for (int i = 0; i < oc; ++i)
  {
    cout << TypeTraits::nameForSignatureType(o.o[i]);
    
    if (i < oc - 1)
      cout << ", ";
  }
  
  cout << endl << "    " << o.desc << endl;
}

bool Help::printHelpForSearch(string search)
{
  multimap<string, OpHelpEntry>::iterator it = operators.begin();
  bool found = false;
  
  while (it != operators.end())
  {
    OpHelpEntry &entry = it->second;
    
    if (entry.ident.find(search) != string::npos || entry.desc.find(search) != string::npos)
    {
      printOperator(it->first, entry);
      found = true;
    }
    
    ++it;
  }
  
  return found;
}


bool Help::printHelpForOperator(string op)
{
  multimap<string, OpHelpEntry>::iterator it = operators.find(op);
  
  if (it != operators.end())
  {
    pair<multimap<string, OpHelpEntry>::iterator,multimap<string, OpHelpEntry>::iterator> rit = operators.equal_range(op);
    
    for (it = rit.first; it != rit.second; ++it)
    {
      OpHelpEntry &o = it->second;      
      printOperator(op, o);
    }
    
    return true;
  }
  
  return false;
}


bool Help::printHelpForType(const std::string& stype)
{
  Type type = TypeTraits::typeForName(stype);
  
  if (type != TYPE_INVALID)
  {
    auto rit = constructors.equal_range(type);
    decltype(rit.first) fit;
    
    if (rit.first != rit.second)
      cout << "available constructors:" << endl;
    
    for (fit = rit.first; fit != rit.second; ++fit)
    {
      const TypeConstructor& c = fit->second;
      
      cout << "  " << c.code << " : " << c.desc << endl;
    }
    
    cout << endl << "available operators:" << endl;
    
    using iterator_t = decltype(operators)::const_iterator;
    std::vector<iterator_t> operations;
    
    multimap<string, OpHelpEntry>::iterator it;
    
    for (it = operators.begin(); it != operators.end(); ++it)
    {
      const OpHelpEntry& o = it->second;
      
      if (o.i[0].isCompatibleWith(type))
        operations.push_back(it);
    }
    
    std::sort(operations.begin(), operations.end(), [] (const iterator_t& i1, const iterator_t& i2) {
      return i1->second.ident < i2->second.ident;
    });
    
    for (const auto& it : operations)
      printOperator(it->first, it->second);
    
    return true;
    
  }
  
  return false;
}

void Help::printOperators()
{
  cout << "  - available operators:" << endl;
  
  multimap<string, OpHelpEntry>::iterator it;
  
  set<string> ops;
  
  for (it = operators.begin(); it != operators.end(); ++it)
    ops.insert(it->first);
  
  set<string>::iterator it2;
  
  cout << "   ";
  int len = 0;
  const int maxLen = 60;

  for (it2 = ops.begin(); it2 != ops.end(); ++it2)
  {
    cout << " " << *it2;
    len += (*it2).length()+1;
    
    if (len > maxLen)
    {
      cout << endl << "   ";
      len = 0;
    }
  }
  
  cout << endl;
}

void Help::printTypes()
{
  cout << "  - available types:" << endl;
  
  set<string> ops;
  
  for (auto it = operators.begin(); it != operators.end(); ++it)
    if (it->second.i[0].isType())
      ops.insert(TypeTraits::nameForSignatureType(it->second.i[0]));
  
  set<string>::iterator it2;

  cout << "    ";
  bool first = true;
  
  for (it2 = ops.begin(); it2 != ops.end(); ++it2)
  {
    if (first) first = false;
    else cout << " ";
    
    cout << *it2;
  }
  
  cout << endl;
}

