#pragma once

#include "defines.h"

#include <regex>

class Regex : public managed_object
{
private:
  std::string _string;
  std::regex _pattern;
  
public:
  Regex(const std::string& pattern) : _string(pattern), _pattern(pattern) { }
  
  bool isFullMatchFor(const std::string& string) { return std::regex_match(string, _pattern); }
  
  
  const std::string& svalue() { return _string; }
  const std::regex& raw() { return _pattern; }
};