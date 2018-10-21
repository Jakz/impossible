/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#include "help.h"

#include <iostream>

multimap<string, OpHelpEntry> Help::operators;
multimap<int, TypeConstructor> Help::constructors;


const char *Help::topicString(Topic topic)
{
  switch (topic)
  {
    case TOPIC_NUMERICS: return "numeric";
    case TOPIC_BITWISE: return "bitwise";
    case TOPIC_STACK: return "stack";
    case TOPIC_COLLECTIONS: return "collections";
    case TOPIC_UTILITY: return "utility";
    case TOPIC_FUNCTIONAL: return "functional";
    case TOPIC_LOGIC: return "logic";
    case TOPIC_TEXT: return "text";
  }
}

void Help::init()
{
  addOperator("+", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "sum", TOPIC_NUMERICS, "addition between integers", "2 2 +", "4"));
  addOperator("+", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "sum", TOPIC_NUMERICS, "addition between floats", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_CHAR, TYPE_INT, TYPE_CHAR, "shift", TOPIC_TEXT, "shift a char by adding an integer value to its value", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_CHAR, TYPE_CHAR, TYPE_STRING, "concatenate", TOPIC_TEXT, "concatenate two strings", "", ""));
  addOperator("+", OpHelpEntry::binary(TYPE_STRING, TYPE_GENERIC, TYPE_STRING, "append", TOPIC_TEXT, "append string representation of value to string", "", ""));
  
  addOperator("+:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic sum", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in lists", "", ""));
  addOperator("+:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic sum", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in arrays", "", ""));
  
  addOperator("+.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic sum", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in the list", "", ""));
  addOperator("+.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic sum", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in the array", "", ""));
  
  addOperator("-:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic sub", TOPIC_COLLECTIONS, "applies - operation between each pair of elements in lists", "", ""));
  addOperator("-:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic sub", TOPIC_COLLECTIONS, "applies - operation between each pair of elements in arrays", "", ""));
  
  addOperator("-.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic sub", TOPIC_COLLECTIONS, "applies - operation between each pair of elements in the list", "", ""));
  addOperator("-.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic sub", TOPIC_COLLECTIONS, "applies - operation between each pair of elements in the array", "", ""));
  
  addOperator("*:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic times", TOPIC_COLLECTIONS, "applies * operation between each pair of elements in lists", "", ""));
  addOperator("*:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic times", TOPIC_COLLECTIONS, "applies * operation between each pair of elements in arrays", "", ""));
  
  addOperator("*.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic times", TOPIC_COLLECTIONS, "applies * operation between each pair of elements in the list", "", ""));
  addOperator("*.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic times", TOPIC_COLLECTIONS, "applies * operation between each pair of elements in the array", "", ""));
  
  addOperator("/:", OpHelpEntry::binary(TYPE_LIST, TYPE_LIST, TYPE_LIST, "diadic div", TOPIC_COLLECTIONS, "applies / operation between each pair of elements in lists", "", ""));
  addOperator("/:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_ARRAY, TYPE_ARRAY, "diadic div", TOPIC_COLLECTIONS, "applies / operation between each pair of elements in arrays", "", ""));
  
  addOperator("/.", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "monadic div", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in the list", "", ""));
  addOperator("/.", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "monadic div", TOPIC_COLLECTIONS, "applies + operation between each pair of elements in the array", "", ""));
  
  addOperator("-", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "sub", TOPIC_NUMERICS, "subtraction between integers", "", ""));
  addOperator("-", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "sub", TOPIC_NUMERICS, "subtraction between floats", "", ""));
  addOperator("-", OpHelpEntry::binary(TYPE_CHAR, TYPE_INT, TYPE_CHAR, "shift", TOPIC_TEXT, "shift a char by subtracting an integer value to its value", "", ""));
  
  addOperator("/", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "div", TOPIC_NUMERICS, "division between integers", "", ""));
  addOperator("/", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "div", TOPIC_NUMERICS, "division between floats", "", ""));
  addOperator("/", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "union", TOPIC_COLLECTIONS, "computes set difference", "", ""));
  
  addOperator("*", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "mul", TOPIC_NUMERICS, "multiplication between integers", "", ""));
  addOperator("*", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "mul", TOPIC_NUMERICS, "multiplication between floats", "", ""));
  
  addOperator("_", OpHelpEntry::unary(TYPE_INT, TYPE_INT, "negation", TOPIC_NUMERICS, "negates integer", "", ""));
  addOperator("_", OpHelpEntry::unary(TYPE_FLOAT, TYPE_FLOAT, "negation", TOPIC_NUMERICS, "negates float", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the list", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_ARRAY, TYPE_ARRAY, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the array", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_STACK, TYPE_STACK, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the stack", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_QUEUE, TYPE_QUEUE, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the queue", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_MAP, TYPE_MAP, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the map", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_STRING, TYPE_LIST, TYPE_INT, "length", TOPIC_COLLECTIONS, "pushes the length of the string", "", ""));
  addOperator("_", OpHelpEntry::unaryO(TYPE_LAZY_ARRAY, TYPE_LAZY_ARRAY, TYPE_INT, "size", TOPIC_COLLECTIONS, "pushes the size of the generated terms of the lazy array", "", ""));
  
  addOperator("//", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "mod", TOPIC_NUMERICS, "computes modulo between integers", "", ""));
  addOperator("//", OpHelpEntry::unaryO(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "modf", TOPIC_NUMERICS, "computes integer and fractional part of a float number", "", ""));
  
  addOperator("<", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_BOOL, "lesser", TOPIC_NUMERICS, "pushes true on stack if first value is lesser than second", "", ""));
  addOperator("<", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_BOOL, "lesser", TOPIC_NUMERICS, "pushes true on stack if first value is lesser than second", "", ""));
  addOperator("<", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "min", TOPIC_COLLECTIONS, "finds minimum element of a list", "", ""));
  addOperator("<", OpHelpEntry::unaryO(TYPE_ARRAY, TYPE_ARRAY, TYPE_GENERIC, "min", TOPIC_COLLECTIONS, "finds minimum element of an array", "", ""));

  
  addOperator(">", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_BOOL, "greater", TOPIC_NUMERICS, "pushes true on stack if first value is greater than second", "", ""));
  addOperator(">", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_BOOL, "greater", TOPIC_NUMERICS, "pushes true on stack if first value is greater than second", "", ""));
  addOperator(">", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "max", TOPIC_COLLECTIONS, "finds maximum element of a list", "", ""));
  addOperator(">", OpHelpEntry::unaryO(TYPE_ARRAY, TYPE_ARRAY, TYPE_GENERIC, "max", TOPIC_COLLECTIONS, "finds maximum element of an array", "", ""));
  
  addOperator("&", OpHelpEntry::binary(TYPE_BOOL, TYPE_BOOL, TYPE_BOOL, "and", TOPIC_LOGIC, "computes logical and between two booleans", "", ""));
  addOperator("&", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "and", TOPIC_BITWISE, "computes bitwise and between two integers", "", ""));
  addOperator("&", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "intersection", TOPIC_COLLECTIONS, "computes set intersection", "", ""));
  
  addOperator("|", OpHelpEntry::binary(TYPE_BOOL, TYPE_BOOL, TYPE_BOOL, "or", TOPIC_LOGIC, "computes logical or between two booleans", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "or", TOPIC_BITWISE, "computes bitwise or between two integers", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_SET, TYPE_SET, TYPE_SET, "union", TOPIC_COLLECTIONS, "computes set union", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_RANGE, TYPE_RANGE, TYPE_RANGE, "union", TOPIC_NUMERICS, "computes union of ranges", "", ""));
  addOperator("|", OpHelpEntry::binary(TYPE_RANGE, TYPE_INT, TYPE_RANGE, "union", TOPIC_NUMERICS, "insert range composed by single integer to range", "", ""));
  
  addOperator("~", OpHelpEntry::unary(TYPE_BOOL, TYPE_BOOL, "not", TOPIC_LOGIC, "computes logical not of a boolean", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_INT, TYPE_INT, "not", TOPIC_BITWISE, "computes bitwise not of an integer", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_FLOAT, TYPE_FLOAT, "reciprocal", TOPIC_NUMERICS, "computes reciprocal (1/n) of float value", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "reverse", TOPIC_COLLECTIONS, "reverses a list", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "reverse", TOPIC_COLLECTIONS, "reverses an array", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_STRING, TYPE_STRING, "reverse", TOPIC_TEXT, "reverses a string", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_RANGE, TYPE_ARRAY, "concretize", TOPIC_COLLECTIONS, "build an explicit integer array from a range", "", ""));
  addOperator("~", OpHelpEntry::unary(TYPE_LAZY_ARRAY, TYPE_ARRAY, "concretize", TOPIC_COLLECTIONS, "build a concrete array with all elements already generated in a lazy array", "", ""));
  
  addOperator(">>", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "right shift", TOPIC_BITWISE, "integer right shift of specified amount", "", ""));
  addOperator(">>", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "filter", TOPIC_COLLECTIONS, "filter list by keeping only values that satisfy predicate", "", ""));
  addOperator(">>", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "filter", TOPIC_COLLECTIONS, "filter array by keeping only values that satisfy predicate", "", ""));
  
  addOperator("<<", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "left shift", TOPIC_BITWISE, "integer left shift of specified amount", "", ""));
  
  addOperator("!", OpHelpEntry::unary(TYPE_LAMBDA, TYPE_UNKNOWN, "apply", TOPIC_FUNCTIONAL, "pop first value from stack and executes it as a lambda", "1 [2+] !", "3"));
  addOperator("!", OpHelpEntry::unary(TYPE_INT, TYPE_FLOAT, "factorial", TOPIC_NUMERICS, "computes factorial", "", ""));
  addOperator("=", OpHelpEntry::binary(TYPE_GENERIC, TYPE_GENERIC, TYPE_BOOL, "eq", TOPIC_LOGIC, "push true on stack if two topmost values are equal", "", ""));
  
  addOperator("^", OpHelpEntry::unary(TYPE_GENERIC, "peek", TOPIC_UTILITY, "prints stack topmost value", "", ""));
  
  addOperator("$", OpHelpEntry::unaryO(TYPE_GENERIC, TYPE_GENERIC, TYPE_GENERIC, "dupe", TOPIC_STACK, "dupe topmost value of stack", "", ""));
  addOperator("%", OpHelpEntry::binaryO(TYPE_GENERIC, TYPE_GENERIC2, TYPE_GENERIC2, TYPE_GENERIC, "swap", TOPIC_STACK, "swap two topmost value of stack", "", ""));
  addOperator(";", OpHelpEntry::unary(TYPE_GENERIC, "drop", TOPIC_STACK, "discard topmost stack element", "", ""));
  addOperator(",", OpHelpEntry::unary(TYPE_INT, TYPE_GENERIC, "pick", TOPIC_STACK, "pick nth element from stack, the index is calculated after popping the integer value", "", ""));
  addOperator("(>", OpHelpEntry::ternaryO2(TYPE_GENERIC, TYPE_GENERIC2, TYPE_GENERIC3, TYPE_GENERIC2, TYPE_GENERIC3, TYPE_GENERIC, "rise", TOPIC_STACK, "rise third value on the top of the stack", "", ""));
  addOperator("s", OpHelpEntry::unary(TYPE_GENERIC, TYPE_STRING, "string cast", TOPIC_TEXT, "convert the value to string", "", ""));
  
  addOperator("?", OpHelpEntry::binary(TYPE_BOOL, TYPE_LAMBDA, TYPE_UNKNOWN, "if", TOPIC_LOGIC, "executes lambda if boolean value is true", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if value is contained in list", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_GENERIC, TYPE_ARRAY, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if value is contained in array", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_STACK, TYPE_GENERIC, TYPE_STACK, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if value is contained in stack", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_QUEUE, TYPE_GENERIC, TYPE_QUEUE, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if value is contained in queue", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_SET, TYPE_GENERIC, TYPE_SET, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if value is contained in set", "", ""));
  addOperator("?", OpHelpEntry::binaryO(TYPE_MAP, TYPE_GENERIC, TYPE_MAP, TYPE_BOOL, "contains", TOPIC_COLLECTIONS, "pushes true if key is contained in map", "", ""));
  
  
  addOperator("??", OpHelpEntry::ternary(TYPE_BOOL, TYPE_LAMBDA, TYPE_LAMBDA, TYPE_UNKNOWN, "if-else", TOPIC_LOGIC, "executes first lambda if boolean is true, second lambda otherwise", "", ""));
  
  addOperator("@", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_INT, TYPE_ARRAY, TYPE_GENERIC, "at", TOPIC_COLLECTIONS, "retrieves n-th element from an array", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_INT, TYPE_ARRAY, TYPE_ARRAY, "extract", TOPIC_COLLECTIONS, "retrieves all the elements specified by the range into a new array", "", ""));
  addOperator("@", OpHelpEntry::unaryO(TYPE_STACK, TYPE_STACK, TYPE_GENERIC, "pop", TOPIC_COLLECTIONS, "pop element from stack", "", ""));
  addOperator("@", OpHelpEntry::unaryO(TYPE_QUEUE, TYPE_QUEUE, TYPE_GENERIC, "pop", TOPIC_COLLECTIONS, "pop element from top of the queue", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_MAP, TYPE_GENERIC, TYPE_MAP, TYPE_GENERIC2, "get", TOPIC_COLLECTIONS, "get element specified by key from map, pushes nil if key is not present", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_LAZY_ARRAY, TYPE_INT, TYPE_LAZY_ARRAY, TYPE_GENERIC, "at", TOPIC_COLLECTIONS, "retrieves n-th element from the lazy array, value will be generated if needed", "", ""));
  addOperator("@", OpHelpEntry::binaryO(TYPE_LAZY_ARRAY, TYPE_RANGE, TYPE_LAZY_ARRAY, TYPE_ARRAY, "extract", TOPIC_COLLECTIONS, "retrieves all the elements specified by the range from the lazy array", "", ""));
  

  addOperator("#", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_INT, TYPE_GENERIC, TYPE_ARRAY, "set", TOPIC_COLLECTIONS, "set n-th element specified by integer of array", "", ""));
  addOperator("#", OpHelpEntry::ternary(TYPE_MAP, TYPE_GENERIC, TYPE_GENERIC2, TYPE_MAP, "set", TOPIC_COLLECTIONS, "set value for key in map", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_SET, TYPE_GENERIC, TYPE_SET, "insert", TOPIC_COLLECTIONS, "insert element in set", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_STACK, TYPE_GENERIC, TYPE_STACK, "push", TOPIC_COLLECTIONS, "push an element on top of the stack", "", ""));
  addOperator("#", OpHelpEntry::binary(TYPE_QUEUE, TYPE_GENERIC, TYPE_QUEUE, "push", TOPIC_COLLECTIONS, "push element back in the queue", "", ""));
  
  addOperator("<#", OpHelpEntry::binary(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, "push front", TOPIC_COLLECTIONS, "push value on top of the list", "", ""));
  addOperator(">#", OpHelpEntry::binary(TYPE_LIST, TYPE_GENERIC, TYPE_LIST, "push back", TOPIC_COLLECTIONS, "push value on back of the list", "", ""));
  addOperator("<@", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "pop front", TOPIC_COLLECTIONS, "pop value from top of the list", "", ""));
  addOperator(">@", OpHelpEntry::unaryO(TYPE_LIST, TYPE_LIST, TYPE_GENERIC, "pop back", TOPIC_COLLECTIONS, "pop value from back of the list", "", ""));

  addOperator(".>", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each element of the list", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each element of the array", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each element of the set", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_STRING, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each character of the string", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_MAP, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each value of the map (keys are not pushed)", "", ""));
  addOperator(".>", OpHelpEntry::binary(TYPE_RANGE, TYPE_LAMBDA, TYPE_UNKNOWN, "each", TOPIC_COLLECTIONS, "executes the lambda for each integer contained in range", "", ""));
  
  addOperator(".>:", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_UNKNOWN, "each with index", TOPIC_COLLECTIONS, "executes the lambda for each element of the array by pushing also the index", "", ""));
  addOperator(".>:", OpHelpEntry::binary(TYPE_MAP, TYPE_LAMBDA, TYPE_UNKNOWN, "each with key", TOPIC_COLLECTIONS, "executes the lambda for each pair of the map by pushing both key and value", "", ""));

  addOperator(".>.", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "map", TOPIC_COLLECTIONS, "applies lambda to each element of the list to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "map", TOPIC_COLLECTIONS, "applies lambda to each element of the array to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_RANGE, TYPE_LAMBDA, TYPE_ARRAY, "map", TOPIC_COLLECTIONS, "applies lambda to each element of the range to build an array", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_STRING, TYPE_LAMBDA, TYPE_STRING, "map", TOPIC_COLLECTIONS, "applies lambda to each character of the string to build a new one", "", ""));
  addOperator(".>.", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_SET, "map", TOPIC_COLLECTIONS, "applies lambda to each element of the set to build a new one", "", ""));

  
  addOperator(":>:", OpHelpEntry::ternary(TYPE_LIST, TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, "dmap", TOPIC_COLLECTIONS, "applies lambda to each pair of elements from both lists to build a new one", "", ""));
  addOperator(":>:", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "dmap", TOPIC_COLLECTIONS, "applies lambda to each pair of elements from both arrays to build a new one", "", ""));
  
  addOperator(":*.", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, "cartesian", TOPIC_COLLECTIONS, "applies lambda to each possible pair of elements from both arrays to build a new one", "", ""));
  addOperator(":*.", OpHelpEntry::ternary(TYPE_RANGE, TYPE_RANGE, TYPE_LAMBDA, TYPE_ARRAY, "cartesian", TOPIC_COLLECTIONS, "applies lambda to each possible pair of elements from both ranges to build a new one", "", ""));


  addOperator(":>.", OpHelpEntry::ternary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", TOPIC_COLLECTIONS, "folds an array by taking accumulator value and passing it through each element", "", ""));
  addOperator(":>.", OpHelpEntry::ternary(TYPE_LIST, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", TOPIC_COLLECTIONS, "folds a list by taking accumulator value and passing it through each element", "", ""));
  addOperator(":>.", OpHelpEntry::ternary(TYPE_SET, TYPE_LAMBDA, TYPE_GENERIC, TYPE_GENERIC, "fold", TOPIC_COLLECTIONS, "folds a set by taking accumulator value and passing it through each element", "", ""));

  
  addOperator(".?", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "min", TOPIC_NUMERICS, "pushes the minimum of two integer numbers", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "min", TOPIC_NUMERICS, "pushes the minimum of two float numbers", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_BOOL, "any", TOPIC_COLLECTIONS, "pushes true on stack if at least one element in the list satisfies the lambda", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_BOOL, "any", TOPIC_COLLECTIONS, "pushes true on stack if at least one element in the array satisfies the lambda", "", ""));
  addOperator(".?", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_BOOL, "any", TOPIC_COLLECTIONS, "pushes true on stack if at least one element in the set satisfies the lambda", "", ""));

  addOperator(":?", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "max", TOPIC_NUMERICS, "pushes the maximum of two integer numbers", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_FLOAT, TYPE_FLOAT, TYPE_FLOAT, "max", TOPIC_NUMERICS, "pushes the maximum of two float numbers", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_LIST, TYPE_LAMBDA, TYPE_BOOL, "every", TOPIC_COLLECTIONS, "pushes true on stack if all the elements in the list satisfies the lambda", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_ARRAY, TYPE_LAMBDA, TYPE_BOOL, "every", TOPIC_COLLECTIONS, "pushes true on stack if all the elements in the array satisfies the lambda", "", ""));
  addOperator(":?", OpHelpEntry::binary(TYPE_SET, TYPE_LAMBDA, TYPE_BOOL, "every", TOPIC_COLLECTIONS, "pushes true on stack if all the elements in the set satisfies the lambda", "", ""));
  
  addOperator("@>", OpHelpEntry::binary(TYPE_INT, TYPE_INT, TYPE_INT, "rand", TOPIC_NUMERICS, "generate a random value between two pushed values (not inclusive of upper bound)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_RANGE, TYPE_INT, "rand", TOPIC_NUMERICS, "extracts a random integer from a range (with uniform probability between all values)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_BOOL, TYPE_BOOL, "rand flag", TOPIC_LOGIC, "generate a random truth value (true or false)", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_GENERIC, "rand get", TOPIC_COLLECTIONS, "extracts a random value from an array", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_LIST, TYPE_GENERIC, "rand get", TOPIC_COLLECTIONS, "extracts a random value from a list", "", ""));
  addOperator("@>", OpHelpEntry::unary(TYPE_SET, TYPE_GENERIC, "rand get", TOPIC_COLLECTIONS, "extracts a random value from a set", "", ""));
  
  addOperator("#>", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "sort", TOPIC_COLLECTIONS, "sorts the specified list", "", ""));
  addOperator("#>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "sort", TOPIC_COLLECTIONS, "sorts the specified array", "", ""));

  addOperator("$>", OpHelpEntry::unary(TYPE_LIST, TYPE_LIST, "shuffle", TOPIC_COLLECTIONS, "shuffles the specified list", "", ""));
  addOperator("$>", OpHelpEntry::unary(TYPE_ARRAY, TYPE_ARRAY, "shuffle", TOPIC_COLLECTIONS, "shuffles the specified array", "", ""));  
  

  addOperator("[#]", OpHelpEntry::nullaryO(TYPE_LAMBDA, "recur", TOPIC_FUNCTIONAL, "pushes lambda in execution on stack", "", ""));
  
  addOperator("<>", OpHelpEntry::binary(TYPE_LAMBDA, TYPE_LAMBDA, TYPE_UNKNOWN, "while", TOPIC_FUNCTIONAL, "executes second lambda while first lambda evaluates to true", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_INT, TYPE_LAMBDA, TYPE_UNKNOWN, "counter", TOPIC_FUNCTIONAL, "executes second lambda a number of times specified by integer", "", ""));
  addOperator("<>", OpHelpEntry::binaryO(TYPE_LIST, TYPE_LAMBDA, TYPE_LIST, TYPE_LIST, "partition", TOPIC_COLLECTIONS, "partition a list according to predicate given by lambda", "", ""));
  addOperator("<>", OpHelpEntry::binaryO(TYPE_ARRAY, TYPE_LAMBDA, TYPE_ARRAY, TYPE_ARRAY, "partition", TOPIC_COLLECTIONS, "partition an array according to predicate given by lambda", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_LAZY_ARRAY, TYPE_LAMBDA, TYPE_LAZY_ARRAY, "generate", TOPIC_COLLECTIONS, "generates terms from a lazy array until predicate over the value becomes false", "", ""));
  addOperator("<>", OpHelpEntry::binary(TYPE_LAZY_ARRAY, TYPE_INT, TYPE_LAZY_ARRAY, "generate", TOPIC_COLLECTIONS, "generates terms until size of the lazy array is equal to specified integer", "", ""));
  addOperator("<>", OpHelpEntry::unary(TYPE_INT, TYPE_GENERIC, "inner get", TOPIC_FUNCTIONAL, "(ONLY AVAILABLE INSIDE A LAZY LIST DECLARATION) fetches a previous element from the lazy list", "", ""));

  
  
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
  
  cout << "Impossible++ v0.2 (build 36) by Jack (jack@pixbits.com)" << endl;
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
  constructors.insert(pair<int, TypeConstructor>(type, constructor));
}

void Help::addOperator(string op, OpHelpEntry entry)
{
  operators.insert(pair<string, OpHelpEntry>(op, entry));
}

void Help::printOperator(string op, OpHelpEntry o)
{
  cout << "  " << op << " (" << o.ident << ") : ";
  
  for (int i = 0; i < o.io; ++i)
  {
    cout << TypeTraits::nameForType(o.i[i]);
    
    if (i < o.io - 1)
      cout << ", ";
    else if (o.oo > 0)
      cout << " -> ";
  }
  
  if (o.io == 0 || o.oo == 0)
    cout << " -> ";
  
  for (int i = 0; i < o.oo; ++i)
  {
    cout << TypeTraits::nameForType(o.o[i]);
    
    if (i < o.oo - 1)
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
    pair<multimap<int, TypeConstructor>::iterator,multimap<int, TypeConstructor>::iterator> rit = constructors.equal_range(type);
    multimap<int, TypeConstructor>::iterator fit;
    
    if (rit.first != rit.second)
      cout << "available constructors:" << endl;
    
    for (fit = rit.first; fit != rit.second; ++fit)
    {
      TypeConstructor &c = fit->second;
      
      cout << "  " << c.code << " : " << c.desc << endl;
    }
    
    cout << endl << "available operators:" << endl;
    
    multimap<string, OpHelpEntry>::iterator it;
    
    for (it = operators.begin(); it != operators.end(); ++it)
    {
      OpHelpEntry &o = it->second;
      
      if (o.i[0] == type)
        printOperator(it->first, o);
    }
    
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
  
  multimap<string, OpHelpEntry>::iterator it;
  
  set<string> ops;
  
  for (it = operators.begin(); it != operators.end(); ++it)
    ops.insert(TypeTraits::nameForType(it->second.i[0]));
  
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

