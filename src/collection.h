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

class String final : public TCollection, public Traits::Indexable, public Traits::Iterable, public Traits::Appendable
{
public:
  using data_t = std::string;
  
private:
  data_t value;
  
  struct iterator
  {
    std::string::const_iterator it;
    Value value;
  };
  
  mutable iterator it;
  
public:
  String() { }
  String(std::string value) : value(value) { }

  virtual void iterate() const override { it.it = value.begin(); }
  virtual bool hasNext() const override { return it.it != value.end(); }
  
  virtual const Value& next() const override;
  
  virtual bool empty() const override { return this->value.empty(); }
  
  integral_t size() const override { return value.length(); }
  Value at(integral_t index) const override { return value[index]; }
  void put(Value v) override { value.append(v.svalue()); }
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

  
  mutable iterator it;
  
public:
  Range(const RangeVector& data) : data(data) { }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return data.empty(); }
  
  virtual void iterate() const override
  {
    it = { 0, 0 };

    if (!empty() > 0)
    {
      it = { this, 0, data[0].a };
    }
    else
    {
      it = { this, 0, -1 };
    }
  }
  
  virtual bool hasNext() const override
  {
    return it.hasNext();
  }

  virtual const Value& next() const override
  {
    if (it.index <= data[it.pair].b)
      it.value = it.index++;
    else if (it.pair < data.size() - 1)
    {
      ++it.pair;
      it.index = data[it.pair].a;
      it.value = it.index++;
    }
    else
      it.value = TYPE_INVALID;
    
    return it.value;
  }
  
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
        return i(v.string()->raw());
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
          const string& v1 = x.string()->raw(), &v2 = y.string()->raw();
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

class List : public TCollection, public Traits::Iterable, public Traits::Appendable
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
  virtual const Value& next() const override
  {
    if (it == data.end())
      return Value::INVALID;
    else
    {
      return *it++;
    }
  }

  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return data.empty(); }
  void put(Value value) override { data.push_back(value); }
  Iterator iterator() const override { return Iterator(new IteratorWrapper<std::list<Value>>(data)); }
  
  const list_t& raw() const { return data; }
  list_t& raw() { return data; }
  
  list_t::iterator begin() { return data.begin(); }
  list_t::iterator end() { return data.end(); }
};


class Tuple : public managed_object, public Traits::Indexable, public Traits::Countable, public Traits::Iterable
{
  using data_t = std::vector<Value>;
private:
  data_t elements;
  
public:
  Tuple(size_t size) : elements(size) { }
  Tuple(Value v) : elements({v}) { }
  Tuple(Value v1, Value v2) : elements({ v1, v2 }) { }
  Tuple(const std::pair<Value, Value>& pair) : Tuple(pair.first, pair.second) { }
  
  Value at(integral_t index) const override { return elements[index]; }
  integral_t size() const override { return elements.size(); }
  Iterator iterator() const override { return Iterator(new IteratorWrapper<data_t>(elements)); }
  
  const data_t& raw() { return elements; }
  
  
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


class Array : public TCollection, public Traits::Indexable, public Traits::Iterable, public Traits::Appendable
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
  virtual const Value& next() const override
  {
    if (it == data.end())
      return Value::INVALID;
    else
    {
      return *it++;
    }
  }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  void put(Value v) override { this->data.push_back(v); }
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
  
  mutable std::vector<Value>::const_iterator it;
  
public:
  LazyArray(const LazyArrayHolder& data) : data(data) { }
  LazyArray(Lambda *lambda, bool useIndices) : data(lambda, useIndices) { }
  
  virtual void iterate() const override { it = data.data().begin(); }
  virtual bool hasNext() const override { return it != data.data().end(); }
  virtual const Value& next() const override
  {
    if (it == data.data().end())
      return Value::INVALID;
    else
      return *it++;
  }
  
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

class Set : public TCollection, public Traits::Iterable
{
public:
  using set_t = std::unordered_set<Value, value_hash>;
  using data_t = set_t;

private:
  set_t data;
  mutable set_t::const_iterator it;
  
public:
  Set(const set_t& data) : data(data) { }
  Set(set_t&& data) : data(data) { }
  Set() { }
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it != data.end(); }
  virtual const Value& next() const override
  {
    if (it == data.end())
      return Value::INVALID;
    else
    {
      return *it++;
    }
  }
  
  virtual void put(Value value) override
  {
    this->data.insert(value);
  }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
  const set_t& raw() const { return data; } //TODO: rotto
  
  Iterator iterator() const override { return Iterator(new IteratorWrapper<data_t>(data)); }
};

#pragma mark Map
class Map : public TCollection, public Traits::Iterable
{
public:
  using map_t = std::unordered_map<Value, Value, value_hash>;
  using data_t = map_t;

private:
  map_t data;
  mutable map_t::const_iterator it;
  
public:
  Map(map_t&& data) : data(data) { };
  Map() { };
  
  virtual void iterate() const override { it = data.begin(); }
  virtual bool hasNext() const override { return it != data.end(); }
  virtual const Value& next() const override
  {
    if (it == data.end())
      return Value::INVALID;
    else
    {
      const Value& v = it->second; //TODO: broken
      ++it;
      return v;
    }
  }
  
  virtual void put(Value value) override
  {
    //this->value->push_back(value);
  }
  
  virtual integral_t size() const override { return data.size(); }
  virtual bool empty() const override { return this->data.empty(); }
  
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
