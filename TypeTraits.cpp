#include "TypeTraits.h"

#include "Value.h"

#include <sstream>
#include <iomanip>

const std::unordered_map<Type, TypeTraits::TypeSpec, enum_hash> TypeTraits::specs =
{
  { TYPE_INT,
    { TYPE_INT, true, false, "int",
      [] (const Value& v) { return std::to_string(v.data.i); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_INT && v2.data.i == v1.data.i; }
    }
  },
  { TYPE_FLOAT,
    { TYPE_FLOAT, true, false, "float",
      [] (const Value& v) {
        stringstream ss(stringstream::out);
        ss << setiosflags(ios::fixed) << setprecision(4) << v.data.f;
        return ss.str();
      },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_FLOAT && v2.data.f == v1.data.f; }
    }
  },
  { TYPE_BOOL,
    {
      TYPE_BOOL, true, false, "bool",
      [] (const Value& v) { return v.data.b ? "true" : "false"; },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_BOOL && v2.data.b == v1.data.b; }
    }
  
  },
  { TYPE_CHAR,
    {
      TYPE_CHAR, true, false, "char",
      [] (const Value& v) { return std::string(1, v.data.c); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_CHAR && v2.data.c == v1.data.c; }
    }
  },
  
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
