/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#include "value.h"
#include "instruction.h"

#include "regexpr.h"
#include "error.h"

#include <iostream>
#include <iomanip>
#include <sstream>

Value::Value(String* string) : type(TYPE_STRING), data(string) { }
Value::Value(Regex* regex) : type(TYPE_REGEX), data(regex) { }
Value::Value(Tuple* tuple) : type(TYPE_TUPLE), data(tuple) { }
Value::Value(List* list) : type(TYPE_LIST), data(list) { }
Value::Value(Stack* stack) : type(TYPE_STACK), data(stack) { }
Value::Value(Queue* queue) : type(TYPE_QUEUE), data(queue) { }
Value::Value(Set* set) : type(TYPE_SET), data(set) { }
Value::Value(Array* array) : type(TYPE_ARRAY), data(array) { }
Value::Value(Map* map) : type(TYPE_MAP), data(map) { }
Value::Value(Range* range) : type(TYPE_RANGE), data(range) { }
Value::Value(Lambda* map) : type(TYPE_LAMBDA), data(map) { }
Value::Value(LazyArray* map) : type(TYPE_LAZY_ARRAY), data(map) { }
Value::Value(Error* error) : type(TYPE_ERROR), data(error) { }



TCollection* Value::collection() const
{
  //TODO: reenable assert(type.isCollection());
  return static_cast<TCollection*>(data.ptr);
}
String* Value::string() const { assert(type == TYPE_STRING); return object<String>(); }
Regex* Value::regex() const { assert(type == TYPE_REGEX); return object<Regex>(); }
Tuple* Value::tuple() const { assert(type == TYPE_TUPLE); return object<Tuple>(); }
List* Value::list() const { assert(type == TYPE_LIST); return object<List>(); }
Stack* Value::stack() const { assert(type == TYPE_STACK); return object<Stack>(); }
Queue* Value::queue() const { assert(type == TYPE_QUEUE); return object<Queue>(); }
Set* Value::set() const { assert(type == TYPE_SET); return object<Set>(); }
Array* Value::array() const { assert(type == TYPE_ARRAY); return object<Array>(); }
Map* Value::map() const { assert(type == TYPE_MAP); return object<Map>(); }
Range* Value::range() const { assert(type == TYPE_RANGE); return object<Range>(); }
Lambda* Value::lambda() const { assert(type == TYPE_LAMBDA); return object<Lambda>(); }
LazyArray* Value::lazyArray() const { assert(type == TYPE_LAZY_ARRAY); return object<LazyArray>(); }
Error* Value::error() const { assert(type == TYPE_ERROR); return object<Error>(); }


Traits::Indexable* Value::indexable() const { return object<Traits::Indexable>(); }
Traits::Iterable* Value::iterable() const { return object<Traits::Iterable>(); }


Value Value::INVALID = Value(TYPE_INVALID);

size_t Value::hash::operator()(const Value& v) const
{
  return v.type.traits().hasher(v);
};

bool Value::operator<(const Value& y) const
{
  auto& x = *this;
  
  if (x.type != y.type)
    return x.type < y.type;
  else
  {
    switch (x.type)
    {
      case TYPE_INT:
      {
        std::less<integral_t> i;
        const integral_t v1 = x.integral(), v2 = x.integral();
        return i(v1, v2);
      }
      case TYPE_FLOAT:
      {
        std::less<real_t> i;
        const real_t v1 = x.real(), v2 = x.real();
        return i(v1, v2);
      }
      case TYPE_CHAR:
      {
        std::less<char> i;
        const char v1 = x.character(), v2 = y.character();
        return i(v1, v2);
      }
      case TYPE_STRING:
      {
        std::less<std::string> i;
        const std::string& v1 = x.string()->raw(), &v2 = y.string()->raw();
        return i(v1, v2);
      }
      case TYPE_BOOL:
      {
        const bool v1 = x.boolean(), v2 = x.boolean();
        return std::less<bool>()(v1, v2);
      }
        // TODO: collezioni
      default: return false;
    }
  }
}