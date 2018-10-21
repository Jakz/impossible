//
//  CompositeTypes.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#ifndef _COMPOSITE_TYPES_H_
#define _COMPOSITE_TYPES_H_

#include "Value.h"
#include "SimpleValue.h"

class String : public TValue<string>, public TCollection
{
private:
  mutable string::const_iterator it;
  
public:
  String(string value) : TValue<string>(TYPE_STRING, value) { }
  String() : TValue<string>(TYPE_STRING, string()) { }
  
  virtual string svalue() const { return value; }
  
  virtual bool equals(const Value *value) const { return this->value.compare(((TValue<string>*)value)->get()) == 0; }
  virtual Value *clone() const { return new String(value); }
  
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
  
  virtual string svalue() const override;
  
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
      this->value.merge(((Int*)value)->get());
    else if (value->type == TYPE_RANGE)
      this->value.rangeUnion(((Range*)value)->get());
  }
};


template<>
struct hash<Value*>
{
  size_t operator() (const Value* v) const {
    switch (v->type)
    {
      case TYPE_INT:  {
        hash<int> i;
        return i(((Int*)v)->get());
      }
      case TYPE_FLOAT: {
        hash<float> i;
        return i(((Float*)v)->get());
      }
      case TYPE_CHAR: {
        hash<char> i;
        return i(((Char*)v)->get());
      }
      case TYPE_STRING: {
        hash<string> i;
        return i(((String*)v)->get());
      }
      case TYPE_BOOL: {
        hash<bool> i;
        return i(((Bool*)v)->get());
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
struct equal_to<Value*>
{
  bool operator() (const Value *v1, const Value *v2) const
  {
    return v1->equals(v2);
  }
};

template<>
struct less<Value*>
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
            less<int> i;
            const int v1 = ((Int*)x)->get(), v2 = ((Int*)y)->get();
            return i(v1, v2);
          }
          case TYPE_FLOAT:
          {
            less<double> i;
            const double v1 = ((Float*)x)->get(), v2 = ((Float*)y)->get();
            return i(v1, v2);
          }
          case TYPE_CHAR:
          {
            less<char> i;
            const char v1 = ((Char*)x)->get(), v2 = ((Char*)y)->get();
            return i(v1, v2);
          }
          case TYPE_STRING:
          {
            less<string> i;
            const string v1 = ((String*)x)->get(), v2 = ((String*)y)->get();
            return i(v1, v2);
          }
          case TYPE_BOOL:
          {
            const bool v1 = ((Bool*)x)->get(), v2 = ((Bool*)y)->get();
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


class List : public CollectionBase<list<Value*>*>
{
private:
  mutable list<Value*>::const_iterator it;
  
public:
  List(list<Value*>* value) : CollectionBase(TYPE_LIST, value) { }
  List() : CollectionBase(TYPE_LIST, new list<Value*>()) { }
  List(Type type, list<Value*>* value) : CollectionBase(type, value) { }
  
  List(Type type) : CollectionBase(type, new list<Value*>()) { }

  
  virtual string svalue() const override;
  
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
  Stack(list<Value*>*value) : List(TYPE_STACK, value) { }
  Stack() : List(TYPE_STACK) { }
  
  virtual string svalue() const;
  
  virtual bool equals(const Value *value) const;
  virtual Value *clone() const;
};

class Queue : public List
{
public:
  Queue(list<Value*>*value) : List(TYPE_QUEUE, value) { }
  Queue() : List(TYPE_QUEUE) { }
  
  virtual string svalue() const;
  
  virtual bool equals(const Value *value) const;
  virtual Value *clone() const;
};


class Array : public TValue<vector<Value*>* >, public TCollection
{
private:
  mutable vector<Value*>::iterator it;
  
public:
  Array(vector<Value*>* value) : TValue<vector<Value*>* >(TYPE_ARRAY, value) { }
  Array() : TValue<vector<Value*>* >(TYPE_ARRAY, new vector<Value*>()) { }
  Array(int size, Value *value) : TValue<vector<Value *>* >(TYPE_ARRAY, new vector<Value*>(size, !value ? new TValue<void*>(TYPE_NIL) : value)) { }
  
  virtual string svalue() const override;
  
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
  mutable vector<Value*>::iterator it;
  
public:
  LazyArray(LazyArrayHolder holder) : TValue<LazyArrayHolder>(TYPE_LAZY_ARRAY, holder) { }
  LazyArray(Lambda *lambda, bool useIndices) : TValue<LazyArrayHolder>(TYPE_LAZY_ARRAY, LazyArrayHolder(lambda, useIndices)) { }
  
  virtual string svalue() const;
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



class Set : public TValue<unordered_set<Value*>* >, public TCollection
{
private:
  mutable unordered_set<Value*>::iterator it;
  
public:
  Set(unordered_set<Value*>* value) : TValue<unordered_set<Value*>* >(TYPE_SET, value) { }
  Set() : TValue<unordered_set<Value*>* >(TYPE_SET, new unordered_set<Value*>()) { }
  
  virtual string svalue() const override;
  
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

class Map : public TValue<unordered_map<Value*, Value*>* >, public TCollection
{
private:
  mutable unordered_map<Value*,Value*>::iterator it;
  
public:
  Map(unordered_map<Value*, Value*>* value) : TValue<unordered_map<Value*, Value*>* >(TYPE_MAP, value) { };
  Map() : TValue<unordered_map<Value *, Value *>* >(TYPE_MAP, new unordered_map<Value*, Value*>()) { };
  
  virtual string svalue() const override;
  
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
  
  virtual string svalue() const;
  
  // TODO
  virtual bool equals(const Value *value) const { return false; }
  virtual Value *clone() const { return new Lambda(value); }
};










struct ValueTree
{
  Value *v;
  ValueTree *n;
  u32 depth;
  
  ValueTree(Value *v, ValueTree *il)
  {
    this->v = v;
    this->n = il;
    
    this->depth = il ? il->depth + 1 : 1;
  }
  
  list<Value*>* assemble()
  {
    list<Value*>* values = new list<Value*>();
    
    ValueTree *tree = this;
    for (int i = 0; i < depth; ++i, tree = tree->n)
      values->push_front(tree->v);
    
    return values;
  }
};

struct ValuePairTree
{
  Value *k;
  Value *v;
  ValuePairTree *n;
  u32 depth;
  
  ValuePairTree(Value *k, Value *v, ValuePairTree *il)
  {
    this->v = v;
    this->k = k;
    this->n = il;
    
    this->depth = il ? il->depth + 1 : 1;
  }
  
  unordered_map<Value*, Value*>* assemble()
  {
    unordered_map<Value*, Value*>* values = new unordered_map<Value*, Value*>;
    
    ValuePairTree *tree = this;
    for (int i = 0; i < depth; ++i, tree = tree->n)
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


#endif /* defined(__Impossible__CompositeTypes__) */
