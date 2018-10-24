/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/21/12
 */

#include "Value.h"
#include "instruction.h"
#include "SimpleValue.h"

#include <iostream>
#include <iomanip>
#include <sstream>

Value::Value(String* string) : type(TYPE_STRING), data(string) { }
Value::Value(List* list) : type(TYPE_STRING), data(list) { }
Value::Value(Stack* stack) : type(TYPE_STRING), data(stack) { }
Value::Value(Queue* queue) : type(TYPE_STRING), data(queue) { }
Value::Value(Set* set) : type(TYPE_STRING), data(set) { }
Value::Value(Array* array) : type(TYPE_STRING), data(array) { }
Value::Value(Map* map) : type(TYPE_STRING), data(map) { }
Value::Value(Range* range) : type(TYPE_RANGE), data(range) { }
Value::Value(Lambda* map) : type(TYPE_LAMBDA), data(map) { }
Value::Value(LazyArray* map) : type(TYPE_LAZY_ARRAY), data(map) { }



TCollection* Value::collection() const
{
  assert(type.isCollection());
  return static_cast<TCollection*>(data.ptr);
}
String* Value::string() const { assert(type == TYPE_STRING); return object<String>(); }
List* Value::list() const { assert(type == TYPE_LIST); return object<List>(); }
Stack* Value::stack() const { assert(type == TYPE_STACK); return object<Stack>(); }
Queue* Value::queue() const { assert(type == TYPE_QUEUE); return object<Queue>(); }
Set* Value::set() const { assert(type == TYPE_SET); return object<Set>(); }
Array* Value::array() const { assert(type == TYPE_ARRAY); return object<Array>(); }
Map* Value::map() const { assert(type == TYPE_MAP); return object<Map>(); }
Range* Value::range() const { assert(type == TYPE_RANGE); return object<Range>(); }
Lambda* Value::lambda() const { assert(type == TYPE_LAMBDA); return object<Lambda>(); }
LazyArray* Value::lazyArray() const { assert(type == TYPE_LAZY_ARRAY); return object<LazyArray>(); }

std::string Value::lvalue()
{
  return svalue() + " : " + type.name();
}


