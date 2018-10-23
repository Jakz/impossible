//
//  CompositeTypes.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#pragma once

#include "Value.h"
#include "SimpleValue.h"

class String : public TCollection
{
private:
  std::string value;
  mutable std::string::const_iterator it;
  
public:
  String() { }
  String(std::string value) : value(value) { }
  
  const std::string& data() const { return value; }
  
  virtual void iterate() const { it = value.begin(); }
  virtual bool hasNext() const { return it != value.end(); }
  
  virtual Value* next() const;
  
  virtual void put(Value *value)
  {
    this->value.append(value->svalue());
  }
  
  virtual u32 size() const { return (u32)value.length(); }
  virtual bool empty() const { return this->value.empty(); }
};

class Range : public TValue<RangeVector>, public TCollection
{
private:
  mutable int currentPair;
  mutable int currentIndex;
  
public:
  Range(RangeVector value) : TValue<RangeVector>(TYPE_RANGE, value) { }
  
  virtual std::string svalue() const override;
  
  // TODO
  virtual bool equals(const Value *value) const override { return false; }
  virtual Value* clone() const override { return new Range(RangeVector(value.data)); }
  
  virtual u32 size() const override { return value.size(); }
  virtual bool empty() const override { return this->value.data->empty(); }
  
  virtual void iterate() const override
  {
    currentPair = 0;
    if (value.data->size() > 0)
      currentIndex = value.data->at(0).a;
    else
    {
      currentIndex = 0;
      currentPair = -1;
    }
  }
  
  virtual bool hasNext() const override
  {
    return currentPair >= 0 && (currentIndex < value.data->at(currentPair).b || currentPair < value.data->size() -1);
  }

  virtual Value* next() const override
  {
    if (currentIndex <= value.data->at(currentPair).b)
      return new Int(currentIndex++);
    else if (currentPair < value.data->size() - 1)
    {
      ++currentPair;
      currentIndex = value.data->at(currentPair).a;
      return new Int(currentIndex++);
    }
    
    return NULL;
  }
  
  virtual void put(Value *value) override
  {
    if (value->type == TYPE_INT)
      this->value.merge(value->as<int>());
    else if (value->type == TYPE_RANGE)
      this->value.rangeUnion(((Range*)value)->get());
  }
};


struct value_hash
{
  size_t operator() (const Value& v) const {
    switch (v.type)
    {
      case TYPE_INT:  {
        std::hash<integral_t> i;
        return i(v.integral());
      }
      case TYPE_FLOAT: {
        std::hash<real_t> i;
        return i(v.real());
      }
      case TYPE_CHAR: {
        std::hash<char> i;
        return i(v.character());
      }
      case TYPE_STRING: {
        std::hash<std::string> i;
        return i(v.string()->data());
      }
      case TYPE_BOOL: {
        std::hash<bool> i;
        return i(v.boolean());
      }
      case TYPE_RANGE: {
        // TODO
        return 0;
      }
        // TODO: collezioni
        
      default: return 0;
    }
  }
};

template<>
struct std::equal_to<Value*>
{
  bool operator() (const Value *v1, const Value *v2) const
  {
    return v1->equals(v2);
  }
};

template<>
struct std::less<Value>
{
  bool operator() (const Value& x, const Value& y) const
  {
    if (x.type != y.type)
      return x.type < y.type;
    else
    {
      switch (x.type)
      {
        case TYPE_INT:
        {
          less<integral_t> i;
          const integral_t v1 = x.integral(), v2 = x.integral();
          return i(v1, v2);
        }
        case TYPE_FLOAT:
        {
          less<real_t> i;
          const real_t v1 = x.real(), v2 = x.real();
          return i(v1, v2);
        }
        case TYPE_CHAR:
        {
          less<char> i;
          const char v1 = x.character(), v2 = y.character();
          return i(v1, v2);
        }
        case TYPE_STRING:
        {
          less<string> i;
          const string& v1 = x.string()->data(), &v2 = y.string()->data();
          return i(v1, v2);
        }
        case TYPE_BOOL:
        {
          const bool v1 = x.boolean(), v2 = x.boolean();
          return less<bool>()(v1, v2);
        }
          // TODO: collezioni
        default: return false;
      }
    }
  }
};


template<typename T>
class CollectionBase : public TValue<T>, public TCollection
{
  
public:
  CollectionBase(Type type, T t) : TValue<T>(type, t) { }
  CollectionBase(Type type) : TValue<T>(type, new T()) { }
};


class List : public TCollection
{
public:
  using list_t = std::list<Value>;
  using utype_t = list_t;
private:
  list_t data;
  
  mutable list_t::const_iterator it;
  
public:
  List() { }
  List(const list_t& data) : data(data) { }
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it !=  data.end(); }
  virtual Value *next() const override
  {
    if (it == data.end())
      return nullptr;
    else
    {
      Value *v = new Value(*it++);
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    data.push_back(value);
  }
  
  virtual u32 size() const override { return (u32)data.size(); }
  virtual bool empty() const override { return data.empty(); }
  
  const list_t& raw() const { return data; }
  list_t& raw() { return data; }
  
  list_t::iterator begin() { return data.begin(); }
  list_t::iterator end() { return data.end(); }
};

class Stack : public List
{
public:
  Stack() : List() { }
  Stack(const std::list<Value>& data) : List(data) { }

};

class Queue : public List
{
public:
  Queue() : List() { }
  Queue(const std::list<Value>& data) : List(data) { }
};


class Array : public TCollection
{
public:
  using array_t = std::vector<Value>;
  using utype_t = array_t;
  
private:
  array_t data;
  mutable array_t::const_iterator it;
  
public:
  Array(array_t& data) : data(data) { }
  Array(array_t&& data) : data(data) { }
  Array() { }
  Array(integral_t size) { data.resize(size); }
  Array(integral_t size, const Value& value) : data(size, value) { }
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it != data.end(); }
  virtual Value *next() const override
  {
    if (it == data.end())
      return NULL;
    else
    {
      Value *v = new Value(*it++);
      return v;
    }
  }
  
  
  virtual void put(Value *value) override
  {
    this->data.push_back(*value);
  }
  
  virtual u32 size() const override { return (u32)data.size(); }
  virtual bool empty() const override { return this->data.empty(); }

  array_t& raw() { return data; }
  const array_t& raw() const { return data; }
  
  array_t::iterator begin() { return data.begin(); }
  array_t::iterator end() { return data.end(); }
};

class LazyArray : public TValue<LazyArrayHolder>, public TCollection
{
private:
  mutable std::vector<Value*>::iterator it;
  
public:
  LazyArray(LazyArrayHolder holder) : TValue<LazyArrayHolder>(TYPE_LAZY_ARRAY, holder) { }
  LazyArray(Lambda *lambda, bool useIndices) : TValue<LazyArrayHolder>(TYPE_LAZY_ARRAY, LazyArrayHolder(lambda, useIndices)) { }
  
  virtual std::string svalue() const;
  //TODO: finire
  virtual bool equals(const Value *value) const { return false; }
  virtual Value* clone() const { return new LazyArray(value); }
  
  virtual void iterate() const { it = value.data()->begin(); }
  virtual bool hasNext() const { return it != value.data()->end(); }
  virtual Value *next() const
  {
    if (it == value.data()->end())
      return NULL;
    else
    {
      Value *v = *it++;
      return v;
    }
  }
  
  
  virtual void put(Value *value)
  {
    // OVERRIDDEN: it's forbidden to write values to a lazy array
  }
  
  virtual u32 size() const { return (u32)value.data()->size(); }
  virtual bool empty() const { return this->value.data()->empty(); }
};



/*template <>
 struct greater<Value*>
 {
 bool operator() (const Value* x, const Value* y)
 {
 return !less<Value*>()(x,y);
 }
 };*/



class Set : public TCollection
{
public:
  using set_t = std::unordered_set<Value, value_hash>;

private:
  set_t data;
  mutable set_t::const_iterator it;
  
public:
  Set(const set_t& data) : data(data) { }
  Set(set_t&& data) : data(data) { }
  Set() { }
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it != data.end(); }
  virtual Value* next() const override
  {
    if (it == data.end())
      return NULL;
    else
    {
      Value *v = new Value(*it++); // TODO: broken
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    this->data.insert(value);
  }
  
  virtual u32 size() const override { return (u32)data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  const set_t& raw() const { return data; } //TODO: rotto
  
};

class Map : public TCollection
{
public:
  using map_t = std::unordered_map<Value, Value, value_hash>;

private:
  map_t data;
  mutable map_t::const_iterator it;
  
public:
  Map(map_t&& data) : data(data) { };
  Map() { };
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it != data.end(); }
  virtual Value *next() const override
  {
    if (it == data.end())
      return NULL;
    else
    {
      Value *v = new Value(it->second); //TODO: broken
      ++it;
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    //this->value->push_back(value);
  }
  
  virtual u32 size() const override { return (u32)data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  map_t& raw() { return data; }
  const map_t& raw() const { return data; }
};

class Lambda : public managed_object
{
private:
  Code* _code;
  
public:
  Lambda(Code* code) : _code(code) { }
  operator Code*() { return _code; }
  
  Code* code() { return _code; }
  
  std::string svalue() const;
};
