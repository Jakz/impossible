//
//  type_utils.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#pragma once

#include <functional>
#include <unordered_map>
#include <string>
#include <cassert>

#include "defines.h"

enum Type : u16;
class Value;
class SignatureType;

class VirtualTable
{
public:
  virtual std::string to_string(const Value& value) { return ""; }
  virtual bool to_bool(const Value&) { return false; }
  virtual bool equal_to(const Value& self, const Value& other) { return false; }
};

class TraitMask
{
private:
  std::underlying_type<Trait>::type traits;
  
public:
  TraitMask(const std::initializer_list<Trait>& traits)
  {
    this->traits = 0U;
    for (const Trait& trait : traits)
      this->traits |= trait;
  }
  
  bool operator&&(Trait trait) const { return (traits & trait) != 0; }
};

class TypeTraits
{
public:
  struct TypeSpec
  {
    Type type;
    TraitMask traits;
    std::string name;
    
    std::function<std::string(const Value& v)> to_string;
    std::function<bool(const Value& v1, const Value& v2)> equal_to;
    
    std::function<bool(const Value&)> to_bool = [] (const Value& ) { return false; };
  };
  
private:
  static const std::unordered_map<Type, TypeSpec, enum_hash> specs;

public:
  
  static const TypeSpec& traits(Type type)
  {
    auto it = specs.find(type);
    assert(it != specs.end());
    return it->second;
  }
  
  static const char* nameForSignatureType(SignatureType type);

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
  TypeInfo(Type type) : type(type)
  {
    assert(type <= TYPE_NONE);
    static_assert(sizeof(TypeInfo) == sizeof(Type), "must be same size");
  }
  //TypeInfo(const TypeInfo& other) : type(other.type) { assert(other.type <= TYPE_NONE); }
      
  inline const char* name() const { return traits().name.c_str(); }
  
  inline const TypeTraits::TypeSpec& traits() const { return TypeTraits::traits(type); }
  
  bool operator==(const TypeInfo& info) const { return this->operator==(info.type); }
  bool operator==(Type type) const { return this->type == type; }
  
  bool operator<(Trait trait) const
  {
    if (this->type == TYPE_NONE)
      return false;
    else if (trait == TRAIT_ANY_TYPE)
      return true;
    else
      return traits().traits && trait;
  }
  
  
  operator Type() const { return type; }
};

class Traits
{
public:
  class Countable
  {
    virtual integral_t size() const = 0;
  };
  
  class Indexable
  {
    virtual const Value& at(integral_t index) = 0;
  };
};

