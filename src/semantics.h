#pragma once

#include "defines.h"
#include "traits.h"
#include "vm.h"

#include <array>

class VM;

struct Arguments
{
  std::array<TypeInfo, 3> t;
  
  Arguments(Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : t({t1, t2, t3}) { }
  
  bool operator==(const Arguments& o) const
  {
    return t[0] == o.t[0]
    && t[1] == o.t[1]
    && t[2] == o.t[2];
  }
  
  /*bool matches(const Arguments& o) const
  {
    return
    (t[0] == o.t[0] || (t[0] == TYPE_GENERIC && o.t[0] != TYPE_NONE)) &&
    (t[1] == o.t[1] || (t[1] == TYPE_GENERIC && o.t[1] != TYPE_NONE)) &&
    (t[2] == o.t[2] || (t[2] == TYPE_GENERIC && o.t[2] != TYPE_NONE));
  }*/
  
  size_t count() const { return std::distance(t.begin(), std::find(t.begin(), t.end(), TYPE_NONE)); }
  
  TypeInfo& operator[](size_t index) { return t[index]; }
  const TypeInfo& operator[](size_t index) const { return t[index]; }
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
  
  Signature(Opcode opcode, Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : opcode(opcode), args(t1, t2, t3) { }
  Signature(Opcode opcode, Arguments&& args) : opcode(opcode), args(args) { }
  bool operator==(const Signature& o) const { return opcode == o.opcode && args == o.args; }
  
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
  
  VariantFunction& operator=(const VariantFunction& o)
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
    
    return *this;

  }
  
  VariantFunction(const VariantFunction& o)
  {
    this->operator=(o);
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
template<> struct type_of<bool> { static constexpr Type value = TYPE_BOOL; };


class Vocabulary
{
public:
  struct Term
  {
    Opcode opcode;
    Arguments input;
    Arguments output;
    
    Term(Opcode opcode, const Arguments& input, const Arguments& output) :
    opcode(opcode), input(input), output(output) { }
  };
  
  std::vector<Term> terms;
  
public:
  
  void add(const Signature& signature, const Arguments& output)
  {
    terms.push_back(Term(signature.opcode, signature.args, output));
  }
  
  void add(const Signature& signature, Type returnType)
  {
    terms.push_back(Term(signature.opcode, signature.args, Arguments(returnType)));
  }
  
  size_t size() const { return terms.size(); }
  decltype(terms)::const_iterator begin() const { return terms.begin(); }
  decltype(terms)::const_iterator end() const { return terms.end(); }
};

class MicroCode
{
private:
  /* mutable for optional caching of signatures */
  mutable std::unordered_map<Signature, VariantFunction, Signature::hash> table;
  
  struct OpcodeData
  {
    bool hasNullary;
    bool hasUnary;
    bool hasBinary;
    bool hasTernary;
    OpcodeData() : hasNullary(false), hasUnary(false), hasBinary(false), hasTernary(false) { }
  };
  
  std::array<OpcodeData, Opcode::OPCODES_COUNT> opcodeData;
  
  const VariantFunction* findBestOverload(Signature os) const
  {
    /* search for perfect match first */
    auto it = table.find(os);
    if (it != table.end()) return &it->second;
    
    /* then try replacing collection types with generic */
    Signature s = Signature(
                  os.opcode,
                  os.args.t[0].isCollection() ? TypeInfo(TYPE_COLLECTION) : os.args.t[0],
                  os.args.t[1].isCollection() ? TypeInfo(TYPE_COLLECTION) : os.args.t[1],
                  os.args.t[2].isCollection() ? TypeInfo(TYPE_COLLECTION) : os.args.t[2]
                  );
    
    it = table.find(s);
    if (it != table.end())
    {
#if CACHE_SIGNATURES_FOR_GENERIC_OPCODES
      /* cache function with that signature for faster retrieval next time */
      table.emplace(std::make_pair(os, it->second));
#endif
      return &it->second;
    }
    
    /* then try with generic types */
    s = Signature(
                  os.opcode,
                  os.args.t[0] != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE,
                  os.args.t[1] != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE,
                  os.args.t[2] != TYPE_NONE ? TYPE_GENERIC : TYPE_NONE
                  );
    it = table.find(s);
    if (it != table.end())
    {
#if CACHE_SIGNATURES_FOR_GENERIC_OPCODES
      /* cache function with that signature for faster retrieval next time */
      table.emplace(std::make_pair(os, it->second));
#endif
      return &it->second;
    }
    
    return nullptr;
  }
  
  void emplace(const Signature& signature, VariantFunction&& function)
  {
    if (table.find(signature) != table.end())
      assert(false);
    
    table.emplace(std::make_pair(signature, function));
  }
  
  Vocabulary _vocabulary;
  
public:
  MicroCode()
  {
    static_assert(sizeof(Signature) == sizeof(u64), "");
  }
  
  const Vocabulary& vocabulary() { return _vocabulary; }
  
  void registerUnary(Signature signature, Arguments retn, const decltype(VariantFunction::unary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasUnary = true;
  }
  
  void registerBinary(Signature signature, Arguments retn, const decltype(VariantFunction::binary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasBinary = true;
  }
  
  void registerNullary(Signature signature, Arguments retn, const decltype(VariantFunction::nullary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasNullary = true;
  }
  
  template<typename T, typename U, typename R, template<typename TT, typename UU, typename RR> class F> void registerNumericTemplate(Opcode opcode)
  {
    registerBinary({ opcode, type_of<T>::value, type_of<U>::value }, type_of<R>::value, [] (VM* vm, const Value& v1, const Value& v2) { vm->push(F<T, U, R>()(v1.number<T>(), v2.number<U>())); });
  }
  
  template<typename T1, bool IS_COMPARISON> using return_type = typename std::conditional<IS_COMPARISON, bool, T1>::type;
  
  template<bool IS_COMPARISON, template<typename TT, typename UU, typename RR> class OP> void registerNumeric(Opcode opcode)
  {
    registerNumericTemplate<real_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<real_t, integral_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, integral_t, return_type<integral_t, IS_COMPARISON>, OP>(opcode);
  }
  
  bool execute(VM* vm, Opcode opcode) const;
  
  void registerDefault();
};
