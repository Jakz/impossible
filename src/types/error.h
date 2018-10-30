#pragma once

#include "defines.h"

#include "heap.h"

enum class ErrorCode
{
  OPERAND_REQUIRED_ON_STACK,
  INVALID_REGEX
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

