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

inline Value::Value(String* string) : type(TYPE_STRING), data(string) { }
inline String* Value::string() const { return object<String>(); }



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


template<>
struct std::hash<Value*>
{
  size_t operator() (const Value* v) const {
    switch (v->type)
    {
      case TYPE_INT:  {
        hash<integral_t> i;
        return i(v->integral());
      }
      case TYPE_FLOAT: {
        hash<real_t> i;
        return i(v->real());
      }
      case TYPE_CHAR: {
        hash<char> i;
        return i(v->character());
      }
      case TYPE_STRING: {
        hash<string> i;
        return i(v->string()->data());
      }
      case TYPE_BOOL: {
        hash<bool> i;
        return i(v->boolean());
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
struct std::less<Value*>
{
  bool operator() (const Value* x, const Value* y)
  {
    if (x->type != y->type)
    {
      return x->type < y->type;
      }
      else
      {
        switch (x->type)
        {
          case TYPE_INT:
          {
            less<integral_t> i;
            const integral_t v1 = x->integral(), v2 = x->integral();
            return i(v1, v2);
          }
          case TYPE_FLOAT:
          {
            less<real_t> i;
            const real_t v1 = x->real(), v2 = x->real();
            return i(v1, v2);
          }
          case TYPE_CHAR:
          {
            less<char> i;
            const char v1 = x->character(), v2 = y->character();
            return i(v1, v2);
          }
          case TYPE_STRING:
          {
            less<string> i;
            const string& v1 = x->string()->data(), &v2 = y->string()->data();
            return i(v1, v2);
          }
          case TYPE_BOOL:
          {
            const bool v1 = x->boolean(), v2 = x->boolean();
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


class List : public CollectionBase<std::list<Value*>*>
{
private:
  mutable std::list<Value*>::const_iterator it;
  
public:
  List(std::list<Value*>* value) : CollectionBase(TYPE_LIST, value) { }
  List() : CollectionBase(TYPE_LIST, new std::list<Value*>()) { }
  List(Type type, std::list<Value*>* value) : CollectionBase(type, value) { }
  
  List(Type type) : CollectionBase(type, new std::list<Value*>()) { }

  
  virtual std::string svalue() const override;
  
  virtual bool equals(const Value *value) const override;
  virtual Value *clone() const override;
  
  virtual void iterate() const override { it = value->begin(); }
  virtual bool hasNext() const override { return it != value->end(); }
  virtual Value *next() const override
  {
    if (it == value->end())
      return NULL;
    else
    {
      Value *v = *it++;
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    this->value->push_back(value);
  }
  
  virtual u32 size() const override { return (u32)value->size(); }
  virtual bool empty() const override { return this->value->empty(); }
};

class Stack : public List
{
public:
  Stack(std::list<Value*>*value) : List(TYPE_STACK, value) { }
  Stack() : List(TYPE_STACK) { }
  
  virtual std::string svalue() const;
  
  virtual bool equals(const Value *value) const;
  virtual Value *clone() const;
};

class Queue : public List
{
public:
  Queue(std::list<Value*>*value) : List(TYPE_QUEUE, value) { }
  Queue() : List(TYPE_QUEUE) { }
  
  virtual std::string svalue() const;
  
  virtual bool equals(const Value *value) const;
  virtual Value *clone() const;
};


class Array : public TValue<std::vector<Value*>* >, public TCollection
{
private:
  mutable std::vector<Value*>::iterator it;
  
public:
  Array(std::vector<Value*>* value) : TValue<std::vector<Value*>* >(TYPE_ARRAY, value) { }
  Array() : TValue<std::vector<Value*>* >(TYPE_ARRAY, new std::vector<Value*>()) { }
  Array(int size, Value *value) : TValue<std::vector<Value *>* >(TYPE_ARRAY, new std::vector<Value*>(size, !value ? new TValue<void*>(TYPE_NIL) : value)) { }
  
  virtual std::string svalue() const override;
  
  virtual bool equals(const Value *value) const override;
  virtual Value *clone() const override;
  
  virtual void iterate() const override { it = value->begin(); }
  virtual bool hasNext() const override { return it != value->end(); }
  virtual Value *next() const override
  {
    if (it == value->end())
      return NULL;
    else
    {
      Value *v = *it++;
      return v;
    }
  }
  
  
  virtual void put(Value *value) override
  {
    this->value->push_back(value);
  }
  
  virtual u32 size() const override { return (u32)value->size(); }
  virtual bool empty() const override { return this->value->empty(); }
  
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



class Set : public TValue<std::unordered_set<Value*>* >, public TCollection
{
private:
  mutable std::unordered_set<Value*>::iterator it;
  
public:
  Set(std::unordered_set<Value*>* value) : TValue<std::unordered_set<Value*>* >(TYPE_SET, value) { }
  Set() : TValue<std::unordered_set<Value*>* >(TYPE_SET, new std::unordered_set<Value*>()) { }
  
  virtual std::string svalue() const override;
  
  virtual bool equals(const Value *value) const override;
  virtual Value *clone() const override;
  
  virtual void iterate() const override { it = value->begin(); }
  virtual bool hasNext() const override { return it != value->end(); }
  virtual Value* next() const override
  {
    if (it == value->end())
      return NULL;
    else
    {
      Value *v = *it++;
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    this->value->insert(value);
  }
  
  virtual u32 size() const override { return (u32)value->size(); }
  virtual bool empty() const override { return this->value->empty(); }
  
};

class Map : public TValue<std::unordered_map<Value*, Value*>* >, public TCollection
{
private:
  mutable std::unordered_map<Value*,Value*>::iterator it;
  
public:
  Map(std::unordered_map<Value*, Value*>* value) : TValue<std::unordered_map<Value*, Value*>* >(TYPE_MAP, value) { };
  Map() : TValue<std::unordered_map<Value *, Value *>* >(TYPE_MAP, new std::unordered_map<Value*, Value*>()) { };
  
  virtual std::string svalue() const override;
  
  // TODO
  virtual bool equals(const Value *value) const override { return false; }
  virtual Value *clone() const override { return new Map(value); }
  
  virtual void iterate() const override { it = value->begin(); }
  virtual bool hasNext() const override { return it != value->end(); }
  virtual Value *next() const override
  {
    if (it == value->end())
      return NULL;
    else
    {
      Value *v = it->second;
      ++it;
      return v;
    }
  }
  
  virtual void put(Value *value) override
  {
    //this->value->push_back(value);
  }
  
  virtual u32 size() const override { return (u32)value->size(); }
  virtual bool empty() const override { return this->value->empty(); }
};

class Lambda : public TValue<Code*>
{
private:
  
public:
  Lambda(Code* value) : TValue<Code*>(TYPE_LAMBDA, value) { };
  
  virtual std::string svalue() const;
  
  // TODO
  virtual bool equals(const Value *value) const { return false; }
  virtual Value *clone() const { return new Lambda(value); }
};










struct ValueTree
{
  Value *v;
  ValueTree *n;
  size_t depth;
  
  ValueTree(Value *v, ValueTree *il)
  {
    this->v = v;
    this->n = il;
    
    this->depth = il ? il->depth + 1 : 1;
  }
  
  std::list<Value*>* assemble()
  {
    std::list<Value*>* values = new std::list<Value*>();
    
    ValueTree *tree = this;
    for (size_t i = 0; i < depth; ++i, tree = tree->n)
      values->push_front(tree->v);
    
    return values;
  }
};

struct ValuePairTree
{
  Value *k;
  Value *v;
  ValuePairTree *n;
  size_t depth;
  
  ValuePairTree(Value *k, Value *v, ValuePairTree *il)
  {
    this->v = v;
    this->k = k;
    this->n = il;
    
    this->depth = il ? il->depth + 1 : 1;
  }
  
  std::unordered_map<Value*, Value*>* assemble()
  {
    std::unordered_map<Value*, Value*>* values = new std::unordered_map<Value*, Value*>;
    
    ValuePairTree *tree = this;
    for (size_t i = 0; i < depth; ++i, tree = tree->n)
      (*values)[tree->k] = tree->v;
    
    return values;
  }
};

struct LambdaIntPair
{
  u32 i;
  Lambda *code;
  LambdaIntPair *n;
  
  LambdaIntPair(u32 i, Lambda *code, LambdaIntPair *next) : i(i), code(code), n(next) { }
};
