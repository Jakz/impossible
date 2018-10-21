//
//  type_utils.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#pragma once

#include <unordered_map>
#include <string>
#include <cassert>

#include "defines.h"

enum Type : s16;

class TypeTraits
{
public:
  struct TypeSpec
  {
    Type type;
    bool isPrimitive;
    bool isCollection;
    std::string name;
  };
  
private:
  static const std::unordered_map<Type, TypeSpec, enum_hash> specs;

public:

  static bool isCollection(Type type)
  {
    auto it = specs.find(type);
    return it != specs.end() ? it->second.isCollection : false;
  }
  
  static const char* nameForType(Type type)
  {
    auto it = specs.find(type);
    if (it != specs.end())
      return it->second.name.c_str();
    assert(false);
  }

  static Type typeForName(const std::string& name)
  {
    for (const auto& spec : specs)
      if (spec.second.name == name)
        return spec.second.type;
    
    return TYPE_INVALID;
  }
};

class TypeInfo
{
private:
  Type type;
  
public:
  TypeInfo(Type type) : type(type) { }
  
  inline const char* name() const { return TypeTraits::nameForType(type); }
  inline bool isCollection() const { return TypeTraits::isCollection(type); }
  
  bool operator==(Type type) const { return this->type == type; }
  bool operator==(const TypeInfo& info) const { return this->type == info.type; }
  
  operator Type() const { return type; }
};
