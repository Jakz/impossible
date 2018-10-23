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

TCollection* Value::collection() const { return static_cast<TCollection*>(data.ptr); }
String* Value::string() const { return object<String>(); }
List* Value::list() const { return object<List>(); }
Stack* Value::stack() const { return object<Stack>(); }
Queue* Value::queue() const { return object<Queue>(); }
Set* Value::set() const { return object<Set>(); }
Array* Value::array() const { return object<Array>(); }
Map* Value::map() const { return object<Map>(); }



const Value* TValue<void*>::NIL = new TValue<void*>(TYPE_NIL);

std::string Value::lvalue()
{
  return svalue() + " : " + type.name();
}


