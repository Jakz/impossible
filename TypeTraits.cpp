#include "TypeTraits.h"

const std::unordered_map<Type, TypeTraits::TypeSpec, enum_hash> TypeTraits::specs =
{
  { TYPE_INT, { TYPE_INT, true, false, "int" } },
  { TYPE_FLOAT, { TYPE_FLOAT, true, false, "float" } },
  { TYPE_BOOL, { TYPE_BOOL, true, false, "bool" } },
  { TYPE_CHAR, { TYPE_CHAR, true, false, "char" } },
  
  { TYPE_STRING, { TYPE_STRING, false, true, "string" } },

  { TYPE_RANGE, { TYPE_RANGE, false, true, "range" } },
  { TYPE_LIST, { TYPE_LIST, false, true, "list" } },
  { TYPE_ARRAY, { TYPE_ARRAY, false, true, "array" } },

  { TYPE_SET, { TYPE_SET, false, true, "set" } },
  { TYPE_STACK, { TYPE_STACK, false, true, "stack" } },
  { TYPE_QUEUE, { TYPE_QUEUE, false, true, "queue" } },

  { TYPE_LAZY_ARRAY, { TYPE_LAZY_ARRAY, false, true, "larray" } },
  
  { TYPE_MAP, { TYPE_MAP, false, true, "map" } },
  { TYPE_LAMBDA, { TYPE_LAMBDA, false, true, "lambda" } },
  { TYPE_NIL, { TYPE_NIL, true, false, "nil" } },

  
  
  { TYPE_GENERIC, { TYPE_GENERIC, true, false, "A" } },
  { TYPE_GENERIC2, { TYPE_GENERIC, true, false, "B" } },
  { TYPE_GENERIC3, { TYPE_GENERIC, true, false, "C" } },
  
  { TYPE_UNKNOWN, { TYPE_UNKNOWN, true, false, "?" } },
};
