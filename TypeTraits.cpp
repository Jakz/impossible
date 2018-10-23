#include "TypeTraits.h"

#include "Value.h"
#include "CompositeValue.h"

#include <sstream>
#include <iomanip>

template<typename T>
struct CollectionPrinter
{
  std::string prefix;
  std::string suffix;
  std::string separator;
  std::function<std::string(const T&)> valuePrinter;
  
  std::string svalue(const TCollection* collection) const
  {
    std::stringstream ss(std::stringstream::out);
    bool first = true;
    collection->iterate();
    
    ss << prefix;
    
    while (collection->hasNext())
    {
      auto it = collection->next();
      
      if (first) first = false;
      else ss << separator;
      
      ss << valuePrinter(it);
    }
    
    ss << suffix;
    
    return ss.str();
  }
};

bool collectionEquals(const Value& v1, const Value& v2)
{
  auto c1 = v1.collection(), c2 = v2.collection();
  
  c1->iterate();
  c2->iterate();
  
  while (c1->hasNext())
  {
    if (!c2->hasNext())
      return false;
  
    if (!c1->next()->equals(c2->next()))
      return false;
  }
  
  return !c2->hasNext();
}

static const CollectionPrinter<Value*> ListPrinter = { "{", "}", " ", [] (const Value* v) { return v->svalue(); } };
static const CollectionPrinter<Value*> StackPrinter = { "{>", "}", " ", [] (const Value* v) { return v->svalue(); } };
static const CollectionPrinter<Value*> QueuePrinter = { "{<", "}", " ", [] (const Value* v) { return v->svalue(); } };
static const CollectionPrinter<Value*> ArrayPrinter = { "(", ")", " ", [] (const Value* v) { return v->svalue(); } };
static const CollectionPrinter<Value*> SetPrinter = { "{.", "}", " ", [] (const Value* v) { return v->svalue(); } };

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
        std::stringstream ss(std::stringstream::out);
        ss << std::setiosflags(std::ios::fixed) << std::setprecision(4) << v.data.f;
        return ss.str();
      },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_FLOAT && v2.data.f == v1.data.f; }
    }
  },
  { TYPE_BOOL,
    {
      TYPE_BOOL, true, false, "bool",
      [] (const Value& v) { return v.data.b ? "true" : "false"; },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_BOOL && v2.data.b == v1.data.b; },
      [] (const Value& v) { return v == TYPE_BOOL && v.data.b; }
    }
  
  },
  { TYPE_CHAR,
    {
      TYPE_CHAR, true, false, "char",
      [] (const Value& v) { return std::string(1, v.data.c); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_CHAR && v2.data.c == v1.data.c; }
    }
  },
  
  { TYPE_STRING,
    { TYPE_STRING, false, true, "string",
      [] (const Value& v) { return v.string()->data(); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_STRING && v2.string()->data() == v1.string()->data(); }
    }
  },

  { TYPE_RANGE, { TYPE_RANGE, false, true, "range" } },
  
  { TYPE_LIST,
    { TYPE_LIST, false, true, "list",
      [] (const Value& v) { return ListPrinter.svalue(v.list()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_LIST && v2.list()->raw() == v1.list()->raw(); }
    }
  },
  
  { TYPE_ARRAY,
    { TYPE_ARRAY, false, true, "array",
      [] (const Value& v) { return ArrayPrinter.svalue(v.queue()); }
    }
  },

  { TYPE_SET,
    { TYPE_SET, false, true, "set",
      [] (const Value& v) { return SetPrinter.svalue(v.queue()); }
    }
  },
  { TYPE_STACK,
    { TYPE_STACK, false, true, "stack",
      [] (const Value& v) { return QueuePrinter.svalue(v.queue()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_STACK && v2.list()->raw() == v1.list()->raw(); }
    }  },
  { TYPE_QUEUE,
    { TYPE_QUEUE, false, true, "queue",
      [] (const Value& v) { return StackPrinter.svalue(v.stack()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_QUEUE && v2.list()->raw() == v1.list()->raw(); }
    }
  },

  { TYPE_LAZY_ARRAY, { TYPE_LAZY_ARRAY, false, true, "larray" } },
  
  { TYPE_MAP, { TYPE_MAP, false, true, "map" } },
  { TYPE_LAMBDA, { TYPE_LAMBDA, false, true, "lambda" } },
  { TYPE_NIL, { TYPE_NIL, true, false, "nil" } },

  
  
  { TYPE_GENERIC, { TYPE_GENERIC, true, false, "A" } },
  { TYPE_GENERIC2, { TYPE_GENERIC, true, false, "B" } },
  { TYPE_GENERIC3, { TYPE_GENERIC, true, false, "C" } },
  
  { TYPE_UNKNOWN, { TYPE_UNKNOWN, true, false, "?" } },
};
