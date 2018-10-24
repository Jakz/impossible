/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#include "instruction.h"

#include "vm.h"

#include <cassert>
#include <array>

static void iter(TCollection *values, Code *code, VM *vm)
{
  values->iterate();
  
  Value v;
  
  while (values->hasNext())
  {
    vm->push(values->next());
    vm->execute(code);
  }
}

static void map(TCollection* values, TCollection* nvalues, Code *code, VM *vm)
{
  values->iterate();
  
  Value o;
  
  while (values->hasNext())
  {
    vm->push(values->next());
    vm->execute(code);
    
    if (vm->popOne(o))
      nvalues->put(o);
  }
}

static void fold(TCollection* values, Value& o, Code *code, VM *vm)
{
  vm->push(o);
  
  values->iterate();
  while (values->hasNext())
  {
    vm->push(values->next());
    vm->execute(code);
  }
  
  vm->popOne(o);
}

static void sfold(TCollection* values, Value& o, Code *code, VM *vm)
{
  values->iterate();
  
  if (values->hasNext())
  {
    Value v = values->next();
    vm->push(v);

    while (values->hasNext())
    {
      vm->push(values->next());
      vm->execute(code);
    }
    
    vm->popOne(o);
  }
}

static void doublemap(TCollection* values1, TCollection* values2, TCollection* nvalues, Code *code, VM *vm)
{
  Value v1, v2;
  Value o;
  
  values1->iterate();
  values2->iterate();
  
  bool running = values1->hasNext() || values2->hasNext();
  
  while (running)
  {
    if (values1->hasNext() && values2->hasNext())
    {
      vm->push(values1->next());
      vm->push(values2->next());

      vm->execute(code);
      
      if (vm->popOne(o))
        nvalues->put(o);
    }
    else if (values1->hasNext())
      nvalues->put(values1->next());
    else if (values2->hasNext())
      nvalues->put(values2->next());
    
    running = values1->hasNext() || values2->hasNext();
  }
}

static void cartesian(TCollection *values1, TCollection* values2, TCollection* nvalues, Code *code, VM *vm)
{
  Value v1, v2;
  Value o;
  
  values1->iterate();
  values2->iterate();

  while (values1->hasNext())
  {
    while (values2->hasNext())
    {
      vm->push(v1);
      vm->push(v2);
      vm->execute(code);
      
      if (vm->popOne(o))
        nvalues->put(o);
    }
    
    values2->iterate();
  }
}

template<typename T>
void filter(VM* vm, const T* collection, Code* predicate)
{
  Value v3;
  const typename T::utype_t& v = collection->raw();
  
  typename T::utype_t::const_iterator it;
  
  typename T::utype_t ot;
  
  for (const auto& value : v)
  {
    vm->push(value);
    vm->execute(predicate);
    
    if (vm->popOne(v3))
    {
      if (v3.type.traits().to_bool(v3))
        ot.push_back(value);
    }
  }
  
  vm->push(new T(ot));
}

struct Arguments
{
  TypeInfo t1, t2, t3;
  
  Arguments(Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : t1(t1), t2(t2), t3(t3) { }
  bool operator==(const Arguments& o) const
  {
    return t1 == o.t1
      && t2 == o.t2
      && t3 == o.t3;
  }
  
  bool matches(const Arguments& o) const
  {
    return
      (t1 == o.t1 || (t1 == TYPE_GENERIC && o.t1 != TYPE_NONE)) &&
      (t2 == o.t2 || (t2 == TYPE_GENERIC && o.t2 != TYPE_NONE)) &&
      (t3 == o.t3 || (t3 == TYPE_GENERIC && o.t3 != TYPE_NONE));
  }
};

struct Signature
{
  union
  {
    struct {
      Opcode opcode;
      Arguments args;
    };
    u64 data;
  };
  
  Signature(Opcode opcode, Type r, Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : opcode(opcode), args(t1, t2, t3) { }
  bool operator==(const Signature& o) const { return opcode == o.opcode && args.matches(o.args); }

  struct hash
  {
  public:
    size_t operator()(const Signature& v) const { return std::hash<u64>()(v.data); }
  };
};

struct VariantFunction
{
  using value_ref = const Value&;
  
  size_t args;
  union
  {
    std::function<void(VM*)> nullary;
    std::function<void(VM*, value_ref)> unary;
    std::function<void(VM*, value_ref, value_ref)> binary;
    std::function<void(VM*, value_ref, value_ref, value_ref)> ternary;
  };
  
  VariantFunction(const decltype(nullary)& nullary) : args(0), nullary(nullary) { }
  VariantFunction(const decltype(unary)& unary) : args(1), unary(unary) { }
  VariantFunction(const decltype(binary)& binary) : args(2), binary(binary) { }
  VariantFunction(const decltype(ternary)& ternary) : args(3), ternary(ternary) { }

  
  VariantFunction(const VariantFunction& o)
  {
    this->args = o.args;
   
    switch (o.args)
    {
      case 0:
        new (&this->nullary) std::function<void(VM*)>();
        this->nullary.operator=(o.nullary);
        break;
      case 1:
        new (&this->unary) std::function<void(VM*, value_ref)>();
        this->unary.operator=(o.unary);
        break;
      case 2:
        new (&this->binary) std::function<void(VM*, value_ref, value_ref)>();
        this->binary = o.binary;
        break;
      case 3:
        new (&this->ternary) std::function<void(VM*, value_ref, value_ref, value_ref)>();
        this->ternary.operator=(o.ternary); break;
      default:
        assert(false);
    }
  }
  
  ~VariantFunction()
  {
    switch (args)
    {
      case 0: nullary.~function<void(VM*)>(); break;
      case 1: unary.~function<void(VM*, value_ref)>(); break;
      case 2: binary.~function<void(VM*, value_ref, value_ref)>(); break;
      case 3: ternary.~function<void(VM*, value_ref, value_ref, value_ref)>(); break;
      default: assert(false);
    }
  }
};

template<typename T> struct type_of { };
template<> struct type_of<integral_t> { static constexpr Type value = TYPE_INT; };
template<> struct type_of<real_t> { static constexpr Type value = TYPE_FLOAT; };


void registerFunctions();
class MicroCode
{
private:
  std::unordered_map<Signature, VariantFunction, Signature::hash> microCode;
  
  struct OpcodeData
  {
    bool hasNullary;
    bool hasUnary;
    bool hasBinary;
    bool hasTernary;
    OpcodeData() : hasNullary(false), hasUnary(false), hasBinary(false), hasTernary(false) { }
  };
  
  std::array<OpcodeData, Opcode::OPCODES_COUNT> opcodeData;
  
  const VariantFunction* findBestOverload(Signature s)
  {
    /* search for perfect match first */
    auto it = microCode.find(s);
    if (it != microCode.end()) return &it->second;
    
    /* then try replacing collection types with generic */
    s = Signature(
                  s.opcode,
                  s.args.t1.isCollection() ? TypeInfo(TYPE_COLLECTION) : s.args.t1,
                  s.args.t2.isCollection() ? TypeInfo(TYPE_COLLECTION) : s.args.t2,
                  s.args.t3.isCollection() ? TypeInfo(TYPE_COLLECTION) : s.args.t3
                  );
    
    it = microCode.find(s);
    if (it != microCode.end()) return &it->second;
    
    /* then try with generic types */
    s = Signature(
                  s.opcode,
                  s.args.t1 != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE,
                  s.args.t2 != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE,
                  s.args.t3 != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE
                  );
    it = microCode.find(s);
    if (it != microCode.end()) return &it->second;
    
    return nullptr;
  }
  
  void emplace(const Signature& signature, VariantFunction&& function)
  {
    if (microCode.find(signature) != microCode.end())
      assert(false);
    
    microCode.emplace(std::make_pair(signature, function));
  }
  
public:
  MicroCode()
  {
    static_assert(sizeof(Signature) == sizeof(u64), "");
    registerFunctions();
    
  }
  
  size_t size() { return microCode.size(); }
  
  void registerUnary(Signature signature, const decltype(VariantFunction::unary)&& function)
  {
    emplace(signature, VariantFunction(function));
    opcodeData[signature.opcode].hasUnary = true;
  }
  
  void registerBinary(Signature signature, const decltype(VariantFunction::binary)&& function)
  {
    emplace(signature, VariantFunction(function));
    opcodeData[signature.opcode].hasBinary = true;
  }
  
  void registerNullary(Signature signature, const decltype(VariantFunction::nullary)&& function)
  {
    emplace(signature, VariantFunction(function));
    opcodeData[signature.opcode].hasNullary = true;
  }

  template<typename T, typename U, typename R, template<typename TT, typename UU, typename RR> class F> void registerNumericTemplate(Opcode opcode)
  {
    registerBinary({ opcode, type_of<T>::value, type_of<U>::value }, [] (VM* vm, const Value& v1, const Value& v2) { vm->push(F<T, U, R>()(v1.number<T>(), v2.number<U>())); });
  }
  
  template<typename T1, bool IS_COMPARISON> using return_type = typename std::conditional<IS_COMPARISON, bool, T1>::type;
  
  template<bool IS_COMPARISON, template<typename TT, typename UU, typename RR> class OP> void registerNumeric(Opcode opcode)
  {
    registerNumericTemplate<real_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<real_t, integral_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, integral_t, return_type<integral_t, IS_COMPARISON>, OP>(opcode);
  }
  
  bool execute(VM* vm, const OpcodeInstruction& instruction)
  {
    const Opcode opcode = instruction.opcode;
    const size_t stackSize = vm->stackSize();
    
    Value v1 = Value::INVALID, v2 = Value::INVALID, v3 = Value::INVALID;
    
    if (opcodeData[opcode].hasTernary && stackSize >= 3)
    {
      v3 = vm->pop();
      v2 = vm->pop();
      v1 = vm->pop();
      
      auto function = findBestOverload({ opcode, v1.type, v2.type, v3.type });
      
      if (function)
      {
        function->ternary(vm, v1, v2, v3);
        return true;
      }
    }
    
    if (opcodeData[opcode].hasBinary && stackSize >= 2)
    {
      if (v3.valid())
      {
        v2 = v3;
        v1 = v2;
      }
      else
      {
        v2 = vm->pop();
        v1 = vm->pop();
      }
      
      auto function = findBestOverload({ opcode, v1.type, v2.type, TYPE_NONE });
      
      if (function)
      {
        function->binary(vm, v1, v2);
        return true;
      }
    }
    
    if (opcodeData[opcode].hasUnary && stackSize >= 1)
    {
      if (v3.valid())
        v1 = v3;
      else if (v2.valid())
        v1 = v2;
      else
        v1 = vm->pop();
      
      auto function = findBestOverload({ opcode, v1.type, TYPE_NONE, TYPE_NONE });
      
      if (function)
      {
        function->unary(vm, v1);
        return true;
      }
    }
    
    auto function = findBestOverload({ opcode, TYPE_NONE, TYPE_NONE, TYPE_NONE });
    
    if (function)
    {
      function->nullary(vm);
      return true;
    }
    
    
    /* push values back */
    if (v1.valid()) vm->push(v1);
    if (v2.valid()) vm->push(v2);
    if (v3.valid()) vm->push(v3);
    
    
    return false;
  }
};


MicroCode microCode;


namespace math
{
  template<typename T, typename U, typename R> struct plus { public: R operator()(T t, U u) { return t + u;} };
  template<typename T, typename U, typename R> struct minus { public: R operator()(T t, U u) { return t - u;} };
  template<typename T, typename U, typename R> struct times { public: R operator()(T t, U u) { return t * u;} };
  template<typename T, typename U, typename R> struct divide { public: R operator()(T t, U u) { return t / u;} };

  template<typename T, typename U, typename R> struct lesser { public: R operator()(T t, U u) { return t < u;} };
  template<typename T, typename U, typename R> struct greater { public: R operator()(T t, U u) { return t > u;} };

}


void registerFunctions()
{
  auto& mc = microCode;
  
  mc.registerNumeric<false, math::plus>(OP_PLUS);
  mc.registerNumeric<false, math::minus>(OP_MINUS);
  mc.registerNumeric<false, math::times>(OP_TIMES);
  mc.registerNumeric<false, math::divide>(OP_DIVIDE);
  
  mc.registerNumeric<true, math::lesser>(OP_LESSER);
  mc.registerNumeric<true, math::greater>(OP_GREATER);

  
  mc.registerUnary({ OP_DUPE, TYPE_GENERIC }, [] (VM* vm, const Value& v1) { vm->push(v1); vm->push(v1); });
  
  mc.registerUnary({ OP_NEG, TYPE_COLLECTION }, [] (VM* vm, const Value& v1) { vm->push(v1.collection()->size()); });
}


void PushInstruction::execute(VM *vm) const
{
  vm->push(value);
}

std::string PushInstruction::svalue() const { return value.svalue(); }


std::string OpcodeInstruction::svalue() const
{
  switch (opcode) {
    case OP_PLUS: return "+";
    case OP_MINUS: return "-";
    case OP_TIMES: return "*";
    case OP_DIVIDE: return "/";
      
    case OP_NEG: return "_";
      
    case OP_MOD: return "//";
      
    case OP_RAND: return "@>";
    case OP_SORT: return "#>";
    case OP_SHUFFLE: return "$>";
      
    case OP_PLUS_MON: return "+.";
    case OP_PLUS_DIA: return "+:";
    case OP_MINUS_MON: return "-.";
    case OP_MINUS_DIA: return "-:";
    case OP_TIMES_MON: return "*.";
    case OP_TIMES_DIA: return "*:";
    case OP_DIVIDE_MON: return "/.";
    case OP_DIVIDE_DIA: return "/:";
      
    case OP_AND: return "&";
    case OP_NOT: return "~";
    case OP_OR: return "|";
    case OP_RSHIFT: return ">>";
    case OP_LSHIFT: return ">>";
      
    case OP_BANG: return "!";
    case OP_DBANG: return "!!";
    case OP_QUESTION: return "?";
    case OP_DQUESTION: return "??";
      
    case OP_EQUAL: return "=";
    case OP_LESSER: return "<";
    case OP_GREATER: return ">";
      
    case OP_DUPE: return "$";
    case OP_SWAP: return "%";
    case OP_DROP: return ";";
    case OP_PICK: return ",";
    case OP_RISE: return "(>";
    case OP_SINK: return "<)";
      
    case OP_PEEK: return "^";
      
    case OP_AT: return "@";
    case OP_HASH: return "#";
    case OP_AT_FRONT: return "<@";
    case OP_AT_BACK: return ">@";
    case OP_HASH_FRONT: return "<#";
    case OP_HASH_BACK: return ">#";
    
    case OP_ITER: return ".>";
    case OP_ITERI: return ".>:";
    case OP_MAP: return ".>.";
    case OP_FOLD: return ":>.";
    case OP_DMAP: return ":>:";
    case OP_CARTESIAN: return ":*.";
      
    case OP_ANY: return ".?";
    case OP_EVERY: return ":?";
      
    case OP_RECUR: return "[#]";
    case OP_RECURV: return "{#}";
    case OP_WHILE: return "<>";
  }
  
  assert(false);
}


void OpcodeInstruction::execute(VM *vm) const
{
  Value v1, v2, v3;
  
  if (microCode.execute(vm, *this))
    return;
  
  switch (opcode)
  {
    case OP_SWAP:
    {
      if (vm->popTwo(v1, v2))
      {
        vm->push(v2);
        vm->push(v1);
      }
      break;
    }
    case OP_DROP:
    {
      if (vm->popOne(v1))
      {
        
      }
      break;
    }
    case OP_PICK:
    {
      if (vm->popOne(v1) && v1.type == TYPE_INT)
      {
        integral_t i = v1.integral();
        vm->push(vm->pick(i));
      }
      break;
    }
    case OP_RISE:
    {
      if (vm->popThree(v1, v2, v3))
      {
        vm->push(v2);
        vm->push(v3);
        vm->push(v1);
      }
      break;
    }
    case OP_SINK:
    {
      if (vm->popThree(v1, v2, v3))
      {
        vm->push(v3);
        vm->push(v1);
        vm->push(v2);
      }
      break;
    }
    
    
    case OP_PLUS:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_STRING)
        {
          vm->push(new Value(new String(v1.string()->data() + v2.svalue())));
        }
        else
        {        
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_CHAR, TYPE_INT): vm->push((char)(v1.character() + v2.integral())); break;
            case TYPES(TYPE_CHAR, TYPE_CHAR): vm->push(new Value(new String(std::string(1,v1.character()) + v2.character()))); break;
          }
        }
      }
      break;
    }
    case OP_MINUS:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_CHAR, TYPE_INT): vm->push((char)(v1.character() - v2.integral())); break;
        }
      }
      break;
    }
    case OP_TIMES:
    {
      break;
    }
    case OP_DIVIDE:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_SET, TYPE_SET):
          {
            Set *s1 = v1.set(), *s2 = v2.set();
            const auto& d1 = s1->raw();
            const auto& d2 = s2->raw();
            Set::set_t result;
            
            for (const auto& it : d1)
              if (d2.find(it) == d2.end())
                result.insert(it);
            
            vm->push(new Value(new Set(result)));
            
            break;
          }           
        }
      }
      
      break;
    }
    case OP_NEG:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)  
        {
          case TYPE_INT: vm->push(-v1.integral()); break;
          case TYPE_FLOAT: vm->push(-v1.real()); break;
          default: break;
        }
      }
      break;
    }
      
      
    case OP_MOD:
    {
      if (vm->popOne(v2))
      {
        switch (v2.type)
        {
          case TYPE_FLOAT:
          {
            double f, i;
            f = modf(v2.real(), &i);
            vm->push(i);
            vm->push(f);
            break;
          }
          default:
          {
            
            if (vm->popOne(v1))
            {
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_INT, TYPE_INT):
                {
                  vm->push(v1.integral() % v2.integral());
                  break;
                }
              }
            }
            break;
          }
        }
      }
      break;
    }
      
    case OP_AND:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_BOOL, TYPE_BOOL): vm->push(v1.boolean() && v2.boolean()); break;
          case TYPES(TYPE_INT, TYPE_INT): vm->push(v1.integral() & v2.integral()); break;
          case TYPES(TYPE_SET, TYPE_SET):
          {
            Set *s1 = v1.set(), *s2 = v2.set();
            const auto& d1 = s1->raw();
            const auto& d2 = s2->raw();
            Set::set_t result;
            
            for (const auto& it : d1)
              if (d2.find(it) != d2.end())
                result.insert(it);
            
            vm->push(new Value(new Set(result)));

            break;
          }
        }
      }
      break;
    }
      
    case OP_OR:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_BOOL, TYPE_BOOL): vm->push(v1.boolean() || v2.boolean()); break;
          case TYPES(TYPE_INT, TYPE_INT): vm->push(v1.integral() | v2.integral()); break;
          case TYPES(TYPE_RANGE, TYPE_RANGE):
          {
            vm->push(new Range(v1.range()->raw().rangeUnion(v2.range()->raw())));
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_INT):
          {
            RangeVector rv = RangeVector(v2.integral(), v2.integral());
            vm->push(new Range(v1.range()->raw().rangeUnion(rv)));
            break;
          }
            
          case TYPES(TYPE_SET, TYPE_SET):
          {
            Set *s1 = v1.set(), *s2 = v2.set();
            const auto& d1 = s1->raw();
            const auto& d2 = s2->raw();
            Set::set_t result;
            
            for (const auto& it : d1)
                result.insert(it);
            
            for (const auto& it : d2)
              result.insert(it);
            
            vm->push(new Value(new Set(result)));
            
            break;
          }
        }
      }
      break;
    }
      
    case OP_NOT:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_BOOL: vm->push(!v1.boolean()); break;
          case TYPE_INT: vm->push(~v1.integral()); break;
          case TYPE_FLOAT: vm->push(1.0 / v1.real()); break;
          case TYPE_LIST:
          {
            const List::list_t& data = v1.list()->raw();
            List::list_t nv = List::list_t(data.rbegin(), data.rend());
            vm->push(new List(nv));
            break;
          }
          case TYPE_ARRAY:
          {
            const Array::array_t& ov = v1.array()->raw();
            Array::array_t nv = Array::array_t(ov.rbegin(), ov.rend());
            vm->push(new Array(nv));
            break;
          }
          case TYPE_STRING:
          {
            const std::string& ov = v1.string()->data();
            std::string nv = std::string(ov.rbegin(), ov.rend());
            vm->push(new Value(TYPE_STRING, new String(nv)));
            break;
          }
          case TYPE_RANGE:
          {
            Array::array_t nv;
            auto iv = v1.range()->raw().concretize();
            
            nv.reserve(iv.size());
            std::transform(iv.begin(), iv.end(), nv.end(), [] (integral_t v) { return Value(v); });

            vm->push(new Array(nv));
            break;
          }
          case TYPE_LAZY_ARRAY:
          {
            const LazyArray::data_t& array = v1.lazyArray()->raw();
            Array::array_t nv;
            nv.reserve(array.size());
            
            for (const auto& v : array)
              nv.push_back(v);
            
            vm->push(v1);
            vm->push(new Array(nv));
            break;
          }
          
          default: break;
        }
      }
      break;
    }
    case OP_RSHIFT:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_INT, TYPE_INT): vm->push(v1.integral() >> v2.integral()); break;
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            filter(vm, v1.list(), v2.lambda()->code());
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            filter(vm, v1.array(), v2.lambda()->code());
            break;
            break;
          }
        }
      }
      break;
    }
    case OP_LSHIFT:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_ARRAY)
        {
          //TODO: undomented instruction?
          /*std::vector<Value*>* vs = ((Array*)v1)->get();
          vs->assign(vs->size(), v2.clone());
          vm->push(v1);
          */
        }
        else
        {
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_INT, TYPE_INT): vm->push(v1.integral() << v2.integral()); break;
          }
        }
      }
      break;
    }
      
      
    case OP_EQUAL:
    {
      if (vm->popTwo(v1, v2))
      {  
        vm->push(v1.equals(v2));
      }
      break;
    }
    case OP_LESSER:
    {
      if (vm->popOne(v2))
      {
        if (v2.type == TYPE_LIST)
        {
          const List::list_t& v = v2.list()->raw();
          
          vm->push(v2);
          
          if (!v.empty())
          {
            List::list_t::const_iterator it = min_element(v.begin(), v.end(), std::less<Value>());
            vm->push(*it);
          }
        }
        else if (v2.type == TYPE_ARRAY)
        {
          const Array::array_t& v = v2.array()->raw();
          
          vm->push(v2);
          
          if (!v.empty())
          {
            Array::array_t::const_iterator it = min_element(v.begin(), v.end(), std::less<Value>());
            vm->push(*it);
          }
        }
      }
      break;
    }
    //TODO min and max should push nil if list is empty?
    case OP_GREATER:
    {
      if (vm->popOne(v2))
      {
        if (v2.type == TYPE_LIST)
        {
          const List::list_t& v = v2.list()->raw();
          vm->push(v2);
          
          if (!v.empty())
          {
            auto it = max_element(v.begin(), v.end(), std::less<Value>());
            vm->push(*it);
          }
        }
        else if (v2.type == TYPE_ARRAY)
        {
          const Array::array_t& v = v2.array()->raw();
          vm->push(v2);
          
          if (!v.empty())
          {
            auto it = max_element(v.begin(), v.end(), std::less<Value>());
            vm->push(*it);
          }
        }
      }
      break;
    }      
      
      
      
    case OP_BANG:
    {
      if (vm->popOne(v1))
      {
        if (v1.type == TYPE_LAMBDA)
          vm->execute(v1.lambda()->code());
        else if (v1.type == TYPE_INT)
        {
          double res = 1.0;
          double v = v1.integral();
          
          while (v > 1)
            res *= v--;
          
          vm->push(res);
        }
      }
      break;
    }
    case OP_QUESTION:
    {
      if (vm->popTwo(v1, v2))
      {
        if (v1.type.isCollection())
        {
          TCollection* collection = v1.collection();
          vm->push(v1);
          vm->push(collection->contains(v2));
        }
        else
        {
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_BOOL,TYPE_LAMBDA):
            {
              if (v1.boolean())
                vm->execute(v2.lambda()->code());
              break;
            }
          }

        }
      }
      break;
    }
    case OP_DQUESTION:
    {
      if (vm->popThree(v1, v2, v3))
      {
        if (v1.type == TYPE_BOOL && v2.type == TYPE_LAMBDA && v3.type == TYPE_LAMBDA)
        {
          if (v1.boolean())
            vm->execute(v2.lambda()->code());
          else
            vm->execute(v3.lambda()->code());
        }
      }
      break;
    }
      

    case OP_PEEK:
    {
      Value v = vm->peek();
      
      if (v.valid())
        std::cout << v.svalue();
      
      break;
    }
      
    case OP_RAND:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_BOOL:
          {
            vm->push((rand()%2 == 0) ? true : false);
            break;
          }
          case TYPE_RANGE:
          {
            auto iv = v1.range()->raw().concretize();
            integral_t c = Util::randi(0, iv.size());
            vm->push(iv[c]);
            break;
          }
          case TYPE_ARRAY:
          case TYPE_LIST:
          case TYPE_SET:
          {
            //TODO: random element

            break;
          }
          default:
          {
            if (vm->popOne(v2))
            {
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_INT, TYPE_INT):
                {
                  integral_t m = v2.integral();
                  integral_t M = v1.integral();
                  
                  if (m > M)
                    std::swap(M, M);

                  vm->push((integral_t)Util::randi(m, M));
                  break;
                }
              }
            }
            break;
          }
        }
        
      }
      break;
    }
      
      //TODO: add support for lambda based
    case OP_SORT:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          {
            v1.list()->raw().sort(std::less<Value>());
            //std::sort(v1.list()->begin(), v1.list()->end());
            vm->push(v1);
            break;
          }
          case TYPE_ARRAY:
          {
            std::sort(v1.array()->begin(), v1.array()->end(), std::less<Value>());
            vm->push(v1);
            break;
          }
 
          default: break;
        }
      }
      break;
    }
      
    case OP_SHUFFLE:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_ARRAY:
          {
            std::random_shuffle(v1.array()->begin(), v1.array()->end());
            vm->push(v1);
            break;
          }
          case TYPE_LIST:
          {
            std::vector<Value> data(v1.list()->begin(), v1.list()->end());
            std::random_shuffle(data.begin(), data.end());
            vm->push(new List(List::list_t(data.begin(), data.end())));
            break;
          }
            
          default: break;
        }
      }
      break;
    }
      
    case OP_AT:
    {
      if (vm->popOne(v2))
      {
        if (v2.type == TYPE_STACK)
        {
          List::list_t& stack = v2.stack()->raw();
          vm->push(v2);
          
          if (!stack.empty())
          {
            vm->push(stack.front());
            stack.pop_front();
          }
        }
        else if (v2.type == TYPE_QUEUE)
        {
          List::list_t& queue = v2.queue()->raw();
          vm->push(v2);
          
          if (!queue.empty())
          {
            vm->push(queue.front());
            queue.pop_front();
          }
        }
        else
        {
          if (vm->popOne(v1))
          {
            if (v1.type == TYPE_MAP)
            {
              const Map::map_t& map = v1.map()->raw();
              vm->push(v1);
              
              auto it = map.find(v2);
              
              vm->push(it != map.end() ? it->second : Value());
            }
            else
            {            
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_ARRAY, TYPE_INT):
                {
                  const Array::utype_t& values = v1.array()->raw();
                  integral_t i = v2.integral();
                  
                  vm->push(v1);
                  vm->push(i < values.size() ? values[i] : Value());
                  
                  break;
                }
                case TYPES(TYPE_ARRAY, TYPE_RANGE):
                {
                  const RangeVector& r = v2.range()->raw();
                  Array::utype_t nv;
                  const Array::utype_t& ov = v1.array()->raw();
                  
                  std::vector<integral_t> iv = r.concretize();
                  nv.reserve(iv.size());
                  
                  for (size_t i = 0; i < iv.size(); ++i)
                  {
                    s64 j = iv[i];
                    
                    if (j < 0)
                      j += ov.size();
                    
                    if (j < ov.size())
                      nv.push_back(ov.at(j));
                  }
                  
                  vm->push(v1);
                  vm->push(new Array(nv));            
                  break;
                }
                case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
                {
                  LazyArray *array = v1.lazyArray();
                  integral_t i = v2.integral();
                  
                  const Value& v = array->raw().at(vm, i);
                  
                  vm->push(v1);
                  vm->push(v);
                  break;
                }
                case TYPES(TYPE_LAZY_ARRAY, TYPE_RANGE):
                {
                  const RangeVector& r = v2.range()->raw();
                  Array::utype_t nv;
                  LazyArray *ov = v1.lazyArray();
                  
                  std::vector<integral_t> iv = r.concretize();
                  nv.reserve(iv.size());
                  
                  for (size_t i = 0; i < iv.size(); ++i)
                  {
                    integral_t j = iv[i];
                    
                    if (j < 0)
                      continue;
                    
                    nv.push_back(ov->raw().at(vm, j));
                  }
                  
                  vm->push(v1);
                  vm->push(new Array(nv));            
                  break;
                }
              }
            }
          }
        }
      }
      break;
    }
      
    case OP_HASH:
    {
      if (vm->popTwo(v2, v3))
      {
        if (v2.type == TYPE_SET)
        {
          Set *set = v2.set();
          set->put(v3);
          vm->push(v2);
        }
        else if (v2.type == TYPE_STACK)
        {
          v2.stack()->raw().push_front(v3);
          vm->push(v2);
        }
        else if (v2.type == TYPE_QUEUE)
        {
          v2.queue()->raw().push_back(v3);
          vm->push(v2);
        }
        else
        {
          if (vm->popOne(v1))
          {
            if (v1.type == TYPE_MAP)
            {
              v1.map()->raw().emplace(std::make_pair(v2, v3));
              vm->push(v1);
            }
            else
            {            
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_ARRAY, TYPE_INT):
                {
                  auto& values = v1.array()->raw();
                  integral_t i = v2.integral();
                  
                  if (i >= values.size() && i >= values.capacity())
                    values.resize(i+1, Value());
                  
                  values[v2.integral()] = v3;
                  
                  vm->push(v1);
                  break;
                }
              }
            }
          }
        }      
      }
      break;
    }
      
    case OP_AT_FRONT:
    {
      if (vm->popOne(v1))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          
          vm->push(v1);
          if (!vv.empty())
          {
            Value nv = vv.front();
            vv.pop_front();
            vm->push(nv);
          }
          else
            vm->push(Value());
        }
      }
      break;
    }
      
    case OP_AT_BACK:
    {
      if (vm->popOne(v1))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          
          vm->push(v1);
          if (!vv.empty())
          {
            Value nv = vv.back();
            vv.pop_back();
            vm->push(nv);
          }
          else
            vm->push(Value());
        }
      }
      break;
    }
      
    case OP_HASH_FRONT:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          vv.push_front(v2);
          vm->push(v1);
        }
      }
      break;
    }
      
    case OP_HASH_BACK:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          vv.push_back(v2);
          vm->push(v1);
        }
      }
      break;
    }
      
    case OP_ITER:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type.isCollection() && v2.type == TYPE_LAMBDA)
        {
          iter(v1.collection(), v2.lambda()->code(), vm);
        }
      }
      break;
    }
      
    case OP_ITERI:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Array* array = v1.array();
            Code *code = v2.lambda()->code();
            
            array->iterate();
            
            integral_t i = 0;
            
            while (array->hasNext())
            {
              auto v = array->next();
              vm->push(i++);
              vm->push(v);
              vm->execute(code);
            }
            break;
          }
          case TYPES(TYPE_MAP, TYPE_LAMBDA):
          {
            const Map::map_t& map = v1.map()->raw();
            Code *c = v2.lambda()->code();
            
            std::unordered_map<Value*, Value*>::iterator it;
            
            for (const auto& it : map)
            {
              vm->push(it.first);
              vm->push(it.second);
              vm->execute(c);
            }
          }
        }
      }
      break;
    }
      
    case OP_MAP:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            Lambda *lambda = v2.lambda();
            List *values = v1.list();
            List *nvalues = new List();

            map(values,nvalues,lambda->code(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Lambda *lambda = v2.lambda();
            Array* values = v1.array();
            Array* nvalues = new Array();
            
            map(values,nvalues,lambda->code(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_LAMBDA):
          {
            Lambda *lambda = v2.lambda();
            Range* values = v1.range();
            Array* nvalues = new Array();
            
            map(values,nvalues,lambda->code(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Lambda *lambda = v2.lambda();
            Set* values = v1.set();
            Set* nvalues = new Set();
            
            map(values,nvalues,lambda->code(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_STRING, TYPE_LAMBDA):
          {
            Lambda *lambda = v2.lambda();
            String* values = v1.string();
            String* nvalues = new String();
            
            map(values,nvalues,lambda->code(),vm);
            
            vm->push(new Value(nvalues));
            break;
          }
        }
      }
      
      break;
    }

    case OP_FOLD:
    {
      if (vm->popThree(v1, v2, v3))
      {
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            fold(v1.collection(), v3, v2.lambda()->code(), vm);
            vm->push(v3);
            break;
          }
        }
      }
      break;
    }
            
    case OP_DMAP:
    {
      if (vm->popThree(v1, v2, v3))
      {
        if (v3.type == TYPE_LAMBDA)
        {
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_LIST, TYPE_LIST):
            {
              List *list = new List();
              doublemap(v1.list(), v2.list(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_LIST, TYPE_ARRAY):
            {
              List *list = new List();
              doublemap(v1.list(), v2.array(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_ARRAY, TYPE_LIST):
            {
              Array *list = new Array();
              doublemap(v1.array(), v2.list(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_ARRAY, TYPE_ARRAY):
            {
              Array *list = new Array();
              doublemap(v1.array(), v2.array(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
          }
        }
        
      }
      break;
    }
      
    case OP_CARTESIAN:
    {
      if (vm->popThree(v1, v2, v3))
      {
        if (v3.type == TYPE_LAMBDA)
        {
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_ARRAY, TYPE_ARRAY):
            {
              Array *list = new Array();
              cartesian(v1.array(), v2.array(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_RANGE, TYPE_RANGE):
            {
              Array *list = new Array();
              cartesian(v1.range(), v2.range(), list, v3.lambda()->code(), vm);
              vm->push(list);
              break;
            }
          }
        }
      }
        
      break;
    }
      
    case OP_ANY:
    {
      if (vm->popTwo(v1, v2))
      {
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_INT, TYPE_INT):
          case TYPES(TYPE_FLOAT, TYPE_FLOAT):
          case TYPES(TYPE_INT, TYPE_FLOAT):
          case TYPES(TYPE_FLOAT, TYPE_INT):
          {
            double x = v1.type == TYPE_INT ? v1.integral() : v1.real();
            double y = v2.type == TYPE_INT ? v2.integral() : v2.real();            
            vm->push(x < y ? v1 : v2);
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Value v = false;
            //TODO: leaks and below
            Code *nc = v2.lambda()->code()->append(new OpcodeInstruction(OP_OR));
            fold(v1.collection(), v, nc, vm);
            vm->push(v);
            break;
          }
        }
      }
      break;
    }
      
    case OP_EVERY:
    {
      if (vm->popTwo(v1, v2))
      {
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_INT, TYPE_INT):
          case TYPES(TYPE_FLOAT, TYPE_FLOAT):
          case TYPES(TYPE_INT, TYPE_FLOAT):
          case TYPES(TYPE_FLOAT, TYPE_INT):
          {
            double x = v1.type == TYPE_INT ? v1.integral() : v1.real();
            double y = v2.type == TYPE_INT ? v2.integral() : v2.real();            
            vm->push(x > y ? v1 : v2);
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Value v = true;
            Code *nc = v2.lambda()->code()->append(new OpcodeInstruction(OP_AND));
            fold(v1.collection(), v, nc, vm);
            vm->push(v);
            break;
          }
        }
      }
      break;
    }
      
    case OP_PLUS_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_PLUS))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_PLUS_MON:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          case TYPE_ARRAY:
          {
            Value o;
            sfold(v1.collection(), o, new CodeStandard(new OpcodeInstruction(OP_PLUS)), vm);
            if (o.valid())
              vm->push(o);
            break;
          }

          default:
            break;
        }
      }
      
      break;
    }
      
    case OP_MINUS_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_MINUS))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_MINUS_MON:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          case TYPE_ARRAY:
          {
            Value o;
            sfold(v1.collection(), o, new CodeStandard(new OpcodeInstruction(OP_MINUS)), vm);
            if (o.valid())
              vm->push(o);
            break;
          }

          default: break;
        }
      }
      
      break;
    }
      
    case OP_TIMES_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_TIMES))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_TIMES_MON:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          case TYPE_ARRAY:
          {
            Value o;
            sfold(v1.collection(), o, new CodeStandard(new OpcodeInstruction(OP_TIMES)), vm);
            if (o.valid())
              vm->push(o);
            break;
          }
          default: break;
        }
      }
      
      break;
    }
      
    case OP_DIVIDE_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_DIVIDE))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_DIVIDE_MON:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          case TYPE_ARRAY:
          {
            Value o;
            sfold(v1.collection(), o, new CodeStandard(new OpcodeInstruction(OP_DIVIDE)), vm);
            if (o.valid())
              vm->push(o);
            break;
          }
          default: break;
        }
      }
      
      break;
    }
      
    case OP_RECUR:
    {
      vm->push(new Lambda(vm->code()));
      break;
    }
    case OP_WHILE:
    {
      if (vm->lazy)
      {
        if (vm->popOne(v1) && v1.type == TYPE_INT)
        {
          vm->push(vm->lazy->at(vm, vm->lazy->index-v1.integral()));
        }
      }
      else if (vm->popTwo(v1, v2))
      {
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_LAMBDA, TYPE_LAMBDA):
          {
            Code *condition = v1.lambda()->code();
            Code *body = v2.lambda()->code();
            
            vm->execute(condition);
            
            while (vm->popOne(v3) && v3.type == TYPE_BOOL && v3.boolean())
            {
              vm->execute(body);
              vm->execute(condition);
            }
            break;
          }
          case TYPES(TYPE_INT, TYPE_LAMBDA):
          {
            Code *c = v2.lambda()->code();
            int j = v1.integral();
            
            for (int i = 0; i < j; ++i)
            {
              vm->execute(c);
            }
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            const auto& values = v1.list()->raw();
            Code *c = v2.lambda()->code();
            std::list<Value*>::iterator it;
            
            List::utype_t ot, of;
            
            for (const auto& v : values)
            {
              vm->push(v);
              vm->execute(c);
              
              if (vm->popOne(v3) && v3.type.traits().to_bool(v3))
                ot.push_back(v);
              else
                of.push_back(v);
            }
            
            vm->push(new List(of));
            vm->push(new List(ot));
            break;
            
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            const auto& values = v1.array()->raw();
            Code *c = v2.lambda()->code();
            std::list<Value*>::iterator it;
            
            Array::utype_t ot, of;
            
            for (const auto& v : values)
            {
              vm->push(v);
              vm->execute(c);
              
              if (vm->popOne(v3) && v3.type.traits().to_bool(v3))
                ot.push_back(v);
              else
                of.push_back(v);
            }
            
            vm->push(new Array(of));
            vm->push(new Array(ot));
            break;
            
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_LAMBDA):
          {
            u32 s = 0;
            LazyArray *lazy = v1.lazyArray();
            Lambda *lambda = v2.lambda();
            bool finished = false;
            
            do {
              const Value& v = lazy->raw().at(vm, s);
              vm->push(v);
              vm->execute(lambda->code());
              
              if (vm->popOne(v3))
              {
                if (v3.type.traits().to_bool(v3))
                  finished = true;
              }
              
              ++s;
              
            } while (!finished);
            
            lazy->raw().shrinkBy(1);
            vm->push(v1);
            
            break;
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
          {
            LazyArray *lazy = v1.lazyArray();
            integral_t i = v2.integral();
            
            for (integral_t j = lazy->raw().size(); j < i; ++j)
              lazy->raw().at(vm, j);

            vm->push(v1);
            
            break;
          }
        }
      }
      break;
    }
      
    case OP_DBANG:
    {
      if (vm->popTwo(v1, v2))
      {
        if (v1.type == TYPE_LAMBDA)
        {
          //vm->push(new Lambda(new CurriedCode(((Lambda*)v1)->get(), v2)));
          break;
        }
      }
      break;
    }
      
    case OP_RAISE_STACK: { vm->raiseStack(); break; }
    case OP_LOWER_STACK: { vm->lowerStack(); break; }
      
    case OP_RAISE_STACKV:
    { 
      if (vm->popOne(v1) && v1.type == TYPE_INT)
      {
        integral_t t = v1.integral();
        
        if (vm->stackSize() >= t)
        {
          std::list<Value> tmpStack;
          
          for (int i = 0; i < t; ++i)
            tmpStack.push_back(vm->pop());
          
          vm->raiseStack();
          
          for (int i = 0; i < t; ++i)
          {
            vm->push(tmpStack.back());
            tmpStack.pop_back();
          }
        }
      }
      break;
    }
      
    case OP_LOWER_STACKV:
    { 
      if (vm->popOne(v1) && v1.type == TYPE_INT)
      {
        integral_t t = v1.integral();
        
        if (vm->stackSize() >= t)
        {
          std::list<Value> tmpStack;
          
          for (int i = 0; i < t; ++i)
            tmpStack.push_back(vm->pop());
          
          vm->lowerStack();
          
          for (int i = 0; i < t; ++i)
          {
            vm->push(tmpStack.back());
            tmpStack.pop_back();
          }
        }
      }
      break;
    }
      
    case OP_STRING_CAST:
    {
      if (vm->popOne(v1))
      {
        vm->push(new Value(new String(v1.svalue())));
      }
    }

  }
}
