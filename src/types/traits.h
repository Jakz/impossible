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

enum Type : s16;
class Value;

class Traits
{
public:
  static constexpr u64 INDEXABLE = 0x01;
  
public:
  class Indexable
  {
    virtual const Value& at(integral_t index) = 0;
  };
};

class VirtualTable
{
public:
  virtual std::string to_string(const Value& value) { return ""; }
  virtual bool to_bool(const Value&) { return false; }
  virtual bool equal_to(const Value& self, const Value& other) { return false; }
};

class TypeTraits
{
public:
  struct TypeSpec
  {
    Type type;
    bool isPrimitive;
    bool isCollection;
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
    return nullptr;
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
  TypeInfo(Type type) : type(type)
  {
    assert(type <= TYPE_NONE);
    static_assert(sizeof(TypeInfo) == sizeof(Type), "must be same size");
  }
  //TypeInfo(const TypeInfo& other) : type(other.type) { assert(other.type <= TYPE_NONE); }
      
  inline const char* name() const { return traits().name.c_str(); }
  inline bool isCollection() const { return traits().isCollection; }
  
  inline const TypeTraits::TypeSpec& traits() const { return TypeTraits::traits(type); }
  
  bool operator==(const TypeInfo& info) const { return this->operator==(info.type); }
  bool operator<(const TypeInfo& info) const { return this->operator<(info.type); }
  bool operator>(const TypeInfo& info) const { return info.operator<(this->type); }
  
  bool operator==(Type type) const { return this->type == type; }
  bool operator<(Type type) const
  {
    if (this->type == TYPE_NONE || type == TYPE_NONE)
      return false;
    else if (type == TYPE_GENERIC)
      return true;
    else
      return type == TYPE_COLLECTION && isCollection();
  }
  
  
  operator Type() const { return type; }
};

