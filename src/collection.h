//
//  CompositeTypes.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#pragma once

#include "value.h"

template<typename T>
class BaseIteratorWrapper : public Iterator::Behavior
{
protected:
  const T& data;
  typename T::const_iterator it;
  
public:
  BaseIteratorWrapper(const T& data) : data(data), it(std::begin(data)) { }
  
  void advance() override { ++it; }
  bool hasNext() const override { return it != data.end(); }
  Value value() const override = 0;
};

template<typename T>
class IteratorWrapper : public BaseIteratorWrapper<T>
{
public:
  IteratorWrapper(const T& data) : BaseIteratorWrapper<T>(data) { }
  Value value() const override { return *this->it; }
};

class String final : public TCollection, public Traits::Indexable, public Traits::Iterable, public Traits::Appendable, public Traits::Poppable
{
public:
  using data_t = std::string;
  using utype_t = data_t;
  
private:
  data_t value;
  
public:
  String() { }
  String(std::string value) : value(value) { }

  virtual bool empty() const override { return this->value.empty(); }
  
  integral_t size() const override { return value.length(); }
  Value at(integral_t index) const override { return value[index]; }
  void put(Value v) override { value.append(v.svalue()); }
  Value pop() override { auto last = value.back(); value.pop_back(); return last; } //TODO: push error if string empty
  Iterator iterator() const override { return Iterator(new IteratorWrapper<std::string>(value)); }
  
  const std::string& raw() const { return value; }
};

class Range : public TCollection, public Traits::Iterable, public Traits::Appendable
{
public:

  
private:
  RangeVector data;
  
  struct iterator
  {
    const Range* range;
    integral_t pair;
    integral_t index;
    
    Value value;
    
    bool hasNext() const
    {
      return pair >= 0 && (index < range->raw()[pair].b || pair < range->raw().size() -1);
    }
  };

public:
  Range(const RangeVector& data) : data(data) { }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return data.empty(); }
  
  virtual void put(Value value) override
  {
    if (value.type == TYPE_INT)
      data.merge(value.integral());
    else if (value.type == TYPE_RANGE)
      data.rangeUnion(value.range()->raw());
    /* TODO: throw exception? */
  }
  
  Iterator iterator() const override
  {
    class Behavior : public Iterator::Behavior
    {
    private:
      const RangeVector& data;
      integral_t pair, index;
      
    public:
      Behavior(const RangeVector& data) : data(data)
      {
        pair = index = 0;
        
        if (!data.empty())
          index = data[0].a;
        else
          index = -1;
      }
      
      void advance() override
      {
        if (index < data[pair].b)
          ++index;
        else if (pair < data.pairCount() - 1)
        {
          ++pair;
          index = data[pair].a;
        }
        else
        {
          /* TODO: not the best solution but it does work for now */
          index = std::numeric_limits<integral_t>::max();
          pair = std::numeric_limits<integral_t>::max();
        }
      }
      
      bool hasNext() const override
      {
        bool v = (index <= data[pair].b || pair < data.pairCount() -1);
        return v;
      }
      
      Value value() const override { return index; }
    };
    
    return Iterator(new Behavior(data));  }
  
  const RangeVector& raw() const { return data; }
};

class List : public TCollection, public Traits::Iterable, public Traits::Appendable, public Traits::Poppable, public Traits::Lookupable
{
public:
  using list_t = std::list<Value>;
  using utype_t = list_t;
protected:
  list_t data;
  
public:
  List() { }
  List(const list_t& data) : data(data) { }
 
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return data.empty(); }
  void put(Value value) override { data.push_back(value); }
  Value pop() override { auto last = data.back(); data.pop_back(); return last; } //TODO: push error if string empty
  Iterator iterator() const override { return Iterator(new IteratorWrapper<std::list<Value>>(data)); }
  std::pair<bool, Value> find(const Value& v) const override { return std::make_pair(std::find(data.begin(), data.end(), v) != data.end(), v); }
  
  const list_t& raw() const { return data; }
  list_t& raw() { return data; }
  
  list_t::iterator begin() { return data.begin(); }
  list_t::iterator end() { return data.end(); }
};

#pragma mark Tuple
class Tuple : public managed_object, public Traits::Indexable, public Traits::Countable, public Traits::Iterable
{
  using data_t = std::vector<Value>;
private:
  data_t elements;
  
public:
  Tuple(size_t size) : elements(size) { }
  Tuple(Value v) : elements({v}) { }
  Tuple(Value v1, Value v2) : elements({ v1, v2 }) { }
  Tuple(const std::pair<const Value, Value>& pair) : Tuple(pair.first, pair.second) { }
  template<typename T> Tuple(const T& data) : elements(data.begin(), data.end()) { }
  
  Value at(integral_t index) const override { return elements[index]; }
  integral_t size() const override { return elements.size(); }
  Iterator iterator() const override { return Iterator(new IteratorWrapper<data_t>(elements)); }
  
  const data_t& raw() { return elements; }
  
  
};

class Stack : public List
{
public:
  //TODO: push and pop are not efficient since they're going front, we sohuld use push_back and iterate it in reverse order
  Stack() : List() { }
  Stack(const std::list<Value>& data) : List(data) { }

  void put(Value value) override { data.push_front(value); }
  Value pop() override { auto last = data.back(); data.pop_front(); return last; } //TODO: push error if string empty

};

class Queue : public List
{
public:
  Queue() : List() { }
  Queue(const std::list<Value>& data) : List(data) { }

  void put(Value value) override { data.push_back(value); }
  Value pop() override { auto last = data.back(); data.pop_back(); return last; } //TODO: push error if string empty
};

#pragma mark Array
class Array : public TCollection, public Traits::Indexable, public Traits::Iterable, public Traits::Poppable, public Traits::Appendable
{
public:
  using array_t = std::vector<Value>;
  using utype_t = array_t;
  
private:
  array_t data;
  
public:
  Array(const array_t& data) : data(data) { }
  Array(array_t&& data) : data(data) { }
  Array() { }
  Array(integral_t size) { data.resize(size); }
  Array(integral_t size, const Value& value) : data(size, value) { }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  void put(Value v) override { this->data.push_back(v); }
  Value pop() override { auto last = data.back(); data.pop_back(); return last; } //TODO: push error if string empty
  Value at(integral_t index) const override { return data[index]; }
  Iterator iterator() const override { return Iterator(new IteratorWrapper<std::vector<Value>>(data)); }
  
  array_t& raw() { return data; }
  const array_t& raw() const { return data; }
  
  array_t::iterator begin() { return data.begin(); }
  array_t::iterator end() { return data.end(); }
  

};

class LazyArray : public TCollection
{
public:
  using data_t = LazyArrayHolder;
  
private:
  LazyArrayHolder data;
  
public:
  LazyArray(const LazyArrayHolder& data) : data(data) { }
  LazyArray(Lambda *lambda, bool useIndices) : data(lambda, useIndices) { }
  
  virtual void put(Value value) override
  {
    // OVERRIDDEN: it's forbidden to write values to a lazy array
  }
  
  //TODO: size not correct I gues
  virtual integral_t size() const override { return data.data().size(); }
  virtual bool empty() const override { return data.data().empty(); }
  
  const LazyArrayHolder& raw() const { return data; }
  LazyArrayHolder& raw() { return data; }

};

class Set : public TCollection, public Traits::Iterable, public Traits::Appendable, public Traits::Lookupable
{
public:
  using set_t = std::unordered_set<Value, Value::hash>;
  using data_t = set_t;

private:
  set_t data;
  
public:
  Set(const set_t& data) : data(data) { }
  Set(set_t&& data) : data(data) { }
  Set() { }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  Iterator iterator() const override { return Iterator(new IteratorWrapper<data_t>(data)); }
  void put(Value value) override { data.insert(value); }
  std::pair<bool, Value> find(const Value& key) const override {
    auto it = data.find(key);
    return std::make_pair<>(it != data.end(), key);
  }
  
  const set_t& raw() const { return data; } //TODO: rotto

};

#pragma mark Map
class Map : public TCollection, public Traits::Iterable, public Traits::Lookupable
{
public:
  using map_t = std::unordered_map<Value, Value, Value::hash>;
  using data_t = map_t;

private:
  map_t data;
  
public:
  Map(map_t&& data) : data(data) { };
  Map() { };
  
  virtual void put(Value value) override
  {
    //this->value->push_back(value);
  }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  std::pair<bool, Value> find(const Value& key) const override {
    auto it = data.find(key);
    if (it != data.end())
      return std::make_pair(true, it->second);
    else
      return std::make_pair(false, Value());
  }
  
  map_t& raw() { return data; }
  const map_t& raw() const { return data; }
  
  Iterator iterator() const override
  {
    class Behavior : public BaseIteratorWrapper<data_t>
    {
    public:
      Behavior(const data_t& data) : BaseIteratorWrapper<data_t>(data) { }
      Value value() const override { return new Tuple(*it); }
    };
    
    return Iterator(new Behavior(data));
  }
};

class Lambda : public managed_object, public Traits::Countable
{
private:
  Code* _code;
  
public:
  Lambda(Code* code) : _code(code) { }
  operator Code*() { return _code; }
  
  Code* code() { return _code; }
  
  void put(size_t index, Value value);
  
  integral_t size() const override;
};
