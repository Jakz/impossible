#include "traits.h"
#include "value.h"
#include "instruction.h"

#include "collection.h"
#include "error.h"
#include "regexpr.h"


#include <sstream>
#include <iomanip>

//TODO: not inlined but requires value.h and this creates circular if in header
Value Iterator::value() const { return behavior->value(); }
Value Iterator::operator*() const { return value(); }


template<typename T>
struct CollectionPrinter
{
  std::string prefix;
  std::string suffix;
  std::string separator;
  std::function<std::string(const T&)> valuePrinter;
  
  std::string svalue(const Traits::Iterable* collection) const
  {
    std::stringstream ss(std::stringstream::out);
    bool first = true;
    Iterator it = collection->iterator();
    
    ss << prefix;
    
    while (it)
    {      
      if (first) first = false;
      else ss << separator;
      
      ss << valuePrinter(*it);
      
      ++it;
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
  
    if (!c1->next().equals(c2->next()))
      return false;
  }
  
  return !c2->hasNext();
}

static const CollectionPrinter<Value> ListPrinter = { "{", "}", " ", [] (const Value& v) { return v.svalue(); } };
static const CollectionPrinter<Value> StackPrinter = { "{>", "}", " ", [] (const Value& v) { return v.svalue(); } };
static const CollectionPrinter<Value> QueuePrinter = { "{<", "}", " ", [] (const Value& v) { return v.svalue(); } };
static const CollectionPrinter<Value> ArrayPrinter = { "(", ")", " ", [] (const Value& v) { return v.svalue(); } };
static const CollectionPrinter<Value> SetPrinter = { "{.", "}", " ", [] (const Value& v) { return v.svalue(); } };
static const CollectionPrinter<Value> MapPrinter = { "{", "}", " ", [] (const Value& v) { return v.tuple()->at(0).svalue() + ':' + v.tuple()->at(1).svalue();; } };

static const CollectionPrinter<Value> TuplePrinter = { "[[", "]]", " ", [] (const Value& v) { return v.svalue(); } };


static const auto unary_false = [](const Value&) { return false; };
static const auto binary_false = [](const Value&, const Value&) { return false; };

const std::unordered_map<Type, TypeTraits::TypeSpec, enum_hash> TypeTraits::specs =
{
  { TYPE_INT,
    { TYPE_INT, {}, "int",
      [] (const Value& v) { return std::to_string(v.data.i); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_INT && v2.data.i == v1.data.i; },
      unary_false
    }
  },
  { TYPE_FLOAT,
    { TYPE_FLOAT, {}, "float",
      [] (const Value& v) {
        std::stringstream ss(std::stringstream::out);
        ss << std::setiosflags(std::ios::fixed) << std::setprecision(4) << v.data.f;
        return ss.str();
      },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_FLOAT && v2.data.f == v1.data.f; },
      unary_false
    }
  },
  { TYPE_BOOL,
    {
      TYPE_BOOL, {}, "bool",
      [] (const Value& v) { return v.data.b ? "true" : "false"; },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_BOOL && v2.data.b == v1.data.b; },
      [] (const Value& v) { return v.type == TYPE_BOOL && v.data.b; }
    }
  
  },
  { TYPE_CHAR,
    {
      TYPE_CHAR, {}, "char",
      [] (const Value& v) { return std::string(1, v.data.c); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_CHAR && v2.data.c == v1.data.c; },
      unary_false
    }
  },
  
  { TYPE_STRING,
    { TYPE_STRING, { TRAIT_COUNTABLE, TRAIT_INDEXABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "string",
      [] (const Value& v) { return v.string()->raw(); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_STRING && v2.string()->raw() == v1.string()->raw(); },
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_STRING, new String()); }
    }
  },
  
  
  { TYPE_REGEX,
    { TYPE_REGEX, { }, "regex",
      [] (const Value& v) { return v.regex()->svalue(); },
      binary_false,
      unary_false,
      
    }
  },

  
  { TYPE_TUPLE,
    { TYPE_TUPLE, { TRAIT_INDEXABLE, TRAIT_COUNTABLE }, "tuple",
      [] (const Value& v) { return TuplePrinter.svalue(v.tuple()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_TUPLE && v2.tuple()->raw() == v2.tuple()->raw(); }
    }
  },

  { TYPE_RANGE,
    { TYPE_RANGE, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "range",
      [] (const Value& v) {
        std::stringstream ss(std::stringstream::out);
        
        bool first = true;
        for (const auto& r : v.range()->raw())
        {
          if (first)
            first = false;
          else
            ss << " ";
          
          ss << r.a << ".." << r.b;
        }
        
        return ss.str();
      },
      binary_false,
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_ARRAY, new Array()); }
    }
  },
  
  { TYPE_LIST,
    { TYPE_LIST, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "list",
      [] (const Value& v) { return ListPrinter.svalue(v.list()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_LIST && v2.list()->raw() == v1.list()->raw(); },
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_LIST, new List()); }
    }
  },
  
  { TYPE_ARRAY,
    { TYPE_ARRAY, { TRAIT_COUNTABLE, TRAIT_INDEXABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "array",
      [] (const Value& v) { return ArrayPrinter.svalue(v.array()); },
      binary_false,
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_ARRAY, new Array()); }
    }
  },

  { TYPE_SET,
    { TYPE_SET, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "set",
      [] (const Value& v) { return SetPrinter.svalue(v.set()); },
      binary_false,
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_SET, new Set()); }
    }
  },
  { TYPE_STACK,
    { TYPE_STACK, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "stack",
      [] (const Value& v) { return StackPrinter.svalue(v.stack()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_STACK && v2.list()->raw() == v1.list()->raw(); },
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_LIST, new List()); }
    }
  },
  { TYPE_QUEUE,
    { TYPE_QUEUE, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "queue",
      [] (const Value& v) { return QueuePrinter.svalue(v.queue()); },
      [] (const Value& v1, const Value& v2) { return v2.type == TYPE_QUEUE && v2.list()->raw() == v1.list()->raw(); },
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_LIST, new List()); }
    }
  },

  { TYPE_LAZY_ARRAY,
    { TYPE_LAZY_ARRAY, { TRAIT_COUNTABLE }, "larray",
      [] (const Value& v) {
        std::string s("(? ");
        s += v.lazyArray()->raw().code()->code()->svalue();
        s += " )";
        return s;
      },
      binary_false,
      unary_false,
      [] (size_t hint) { return std::make_pair(TYPE_ARRAY, new Array()); }
    }
  },
  
  { TYPE_MAP,
    { TYPE_MAP, { TRAIT_COUNTABLE, TRAIT_ITERABLE, TRAIT_APPENDABLE }, "map",
      [] (const Value& v) { return MapPrinter.svalue(v.map()); },

    }
  },
  
  { TYPE_LAMBDA,
    { TYPE_LAMBDA, { TRAIT_COUNTABLE }, "lambda",
      [] (const Value& v) {
        std::stringstream ss(std::stringstream::out);
        size_t size = v.lambda()->code()->size();
        
        ss << "[";
        
        for (size_t i = 0; i < size; ++i)
        {
          const Instruction& in = v.lambda()->code()->at(i);
          ss << in.svalue();
        }
        
        ss << "]";
        
        return ss.str();
      },

    }
  },
  
  { TYPE_ERROR,
    { TYPE_ERROR, {}, "error",
      [] (const Value& v) { return "error(" + v.error()->message() + ")"; }
    }
    
  },
  
  { TYPE_NIL,
    { TYPE_NIL, {}, "nil",
      [] (const Value& v) { return "nil"; }
    }
  
  },

  
  
  { TYPE_GENERIC, { TYPE_GENERIC, {}, "A" } },
  { TYPE_GENERIC2, { TYPE_GENERIC, {}, "B" } },
  { TYPE_GENERIC3, { TYPE_GENERIC, {}, "C" } },
  
  { TYPE_COLLECTION, { TYPE_COLLECTION, {}, "collection" } },

  
  { TYPE_UNKNOWN, { TYPE_UNKNOWN, {}, "?" } },
  
  { TYPE_INVALID, { TYPE_INVALID, {}, "invalid" } },
  { TYPE_NONE, { TYPE_NONE, {}, "none" } }
};


#include "semantics.h"

std::string TypeTraits::nameForTrait(Trait trait)
{
  std::string result;

  static const std::unordered_map<Trait, std::string, enum_hash> names = {
    { Trait::TRAIT_COUNTABLE, "countable" },
    { Trait::TRAIT_INDEXABLE, "indexable" },
    { Trait::TRAIT_ITERABLE, "iterable" },
    { Trait::TRAIT_APPENDABLE, "appendable" },
  };
  
  for (const auto& e : names)
  {
    if (e.first & trait)
    {
      if (!result.empty())
        result += "&";
      result += e.second;
    }
  }
  
  return result;
}

std::string TypeTraits::nameForSignatureType(SignatureType type)
{
  if (type.isType())
  {
    auto it = specs.find(type.type());
    if (it != specs.end())
      return it->second.name.c_str();
    
    assert(false);
    return nullptr;
  }
  else
  {
    Trait trait = type.trait();
    
    if (trait >= TRAIT_ANY_TYPE && trait <= TRAIT_ANY_TYPE_LAST)
      return std::string(1, 'A' + (trait - TRAIT_ANY_TYPE));
    
    std::string name = nameForTrait(trait);
    
    assert(!name.empty());
    return name;
  }
}
