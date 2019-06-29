#pragma once

#include "defines.h"

#include "vm/heap.h"

enum class ErrorCode
{
  OPERAND_REQUIRED_ON_STACK,
  VALUE_REQUIRED_IN_ITERABLE,
  INVALID_REGEX,
  INDEX_OUT_OF_BOUNDS
};

class Error : public managed_object
{
private:
  ErrorCode _code;
  std::string _message;
  
public:
  Error(ErrorCode code) : _code(code) { }
  Error(ErrorCode code, const std::string& message) : _code(code), _message(message) { }
  
  const std::string& message() { return _message; }
};

#if IMPOSSIBLE_USE_EXCEPTIONS

class OperandRequiredOnStackException : public std::exception
{
  
};

#define IMPOSSIBLE_THROW(x) throw x;
#else
#define IMPOSSIBLE_THROW(x)
#endif
