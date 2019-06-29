#pragma once

#include "defines.h"
#include "types/traits.h"
#include "vm.h"

#include <array>

class VM;

struct SignatureType
{
private:
  u32 value;
  
public:
  SignatureType(Type type) : value(TRAIT_SPECIFIC_TYPE | type) { }
  SignatureType(Trait trait) : value(trait) { }
  
  bool operator==(const SignatureType& other) const { return value == other.value; }
  
  bool isType() const { return value & TRAIT_SPECIFIC_TYPE; }
  Trait trait() const { return static_cast<Trait>(value); }
  Type type() const { return (Type)(value & TYPE_MASK); }
  
  bool operator==(Type type) const { return value == (TRAIT_SPECIFIC_TYPE | type); }
  bool operator&&(Trait trait) const { return ((value & ~TRAIT_SPECIFIC_TYPE) & trait) != 0; }
  
  bool operator!=(Type type) const { return !this->operator==(type); }
  
  
  bool isCompatibleWith(Type type) const
  {
    return (isType() && this->type() == type) || (!isType() && TypeInfo(type).operator<(trait()));
  }

  struct hash
  {
    size_t operator()(const SignatureType& v) const { return static_cast<size_t>(v.value); }
  };
};

using ArgumentType = Type;

struct SignatureArguments
{
public:
  std::array<SignatureType, 3> t;

public:
  SignatureArguments(SignatureType t1 = TYPE_NONE, SignatureType t2 = TYPE_NONE, SignatureType t3 = TYPE_NONE) : t({t1, t2, t3}) { }
  size_t count() const { return std::distance(t.begin(), std::find(t.begin(), t.end(), TYPE_NONE)); }

  SignatureType& operator[](size_t index) { return t[index]; }
  const SignatureType& operator[](size_t index) const { return t[index]; }

  bool operator==(const SignatureArguments& other) const { return t == other.t; }
};

struct ActualArguments
{
public:
  std::array<TypeInfo, 3> t;
  
public:
  ActualArguments(Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : t({t1, t2, t3}) { }
  
  bool operator==(const SignatureArguments& other) const
  {
    for (size_t i = 0; i < t.size(); ++i)
      if (other.t[i] != t[i])
        return false;
    return true;
  }
  
  bool operator<(const SignatureArguments& o) const
  {
    for (size_t i = 0; i < t.size(); ++i)
    {
      bool typeMatch = o.t[i].isType() && o.t[i] == t[i];
      bool traitMatch = !o.t[i].isType() && t[i] < o.t[i].trait();
      
      if (!typeMatch & !traitMatch)
        return false;
    }
    
    return true;
  }
  
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
      SignatureArguments args;
    };
    u64 data;
  };
  
  Signature(Opcode opcode, SignatureType t1 = TYPE_NONE, SignatureType t2 = TYPE_NONE, SignatureType t3 = TYPE_NONE) : opcode(opcode), args(t1, t2, t3) { }
  Signature(Opcode opcode, SignatureArguments&& args) : opcode(opcode), args(args) { }
  bool operator==(const Signature& o) const { return opcode == o.opcode && args == o.args; }
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
    SignatureArguments input;
    SignatureArguments output;
    
    Term(Opcode opcode, const SignatureArguments& input, const SignatureArguments& output) :
    opcode(opcode), input(input), output(output) { }
  };
  
  std::vector<Term> terms;
  
public:
  
  void add(const Signature& signature, const SignatureArguments& output)
  {
    terms.push_back(Term(signature.opcode, signature.args, output));
  }
  
  void add(const Signature& signature, Type returnType)
  {
    terms.push_back(Term(signature.opcode, signature.args, SignatureArguments(returnType)));
  }
  
  size_t size() const { return terms.size(); }
  decltype(terms)::const_iterator begin() const { return terms.begin(); }
  decltype(terms)::const_iterator end() const { return terms.end(); }
};

struct ActualSignature
{
  union
  {
    struct {
      Opcode opcode;
      ActualArguments args;
    };
    u64 data;
  };
  
  ActualSignature(Opcode opcode, Type t1 = TYPE_NONE, Type t2 = TYPE_NONE, Type t3 = TYPE_NONE) : opcode(opcode), args({t1, t2, t3}) { }

  
  bool operator==(const ActualSignature& other) const { return data == other.data; }
  
  struct hash
  {
  public:
    size_t operator()(const ActualSignature& v) const { return std::hash<u64>()(v.data); }
  };
};

class MicroCode
{
private:
  mutable std::unordered_map<Opcode, std::vector<std::pair<Signature, VariantFunction>>> table;
  /* mutable for optional caching of signatures */
  mutable std::unordered_map<ActualSignature, const VariantFunction*, ActualSignature::hash> cache;
  
  struct OpcodeData
  {
    bool hasNullary;
    bool hasUnary;
    bool hasBinary;
    bool hasTernary;
    OpcodeData() : hasNullary(false), hasUnary(false), hasBinary(false), hasTernary(false) { }
  };
  
  std::array<OpcodeData, Opcode::OPCODES_COUNT> opcodeData;
  
  const VariantFunction* findBestOverload(ActualSignature os) const
  {
    auto cit = cache.find(os);
    if (cit != cache.end())
      return cit->second;
    
    const VariantFunction* matching = nullptr;
    
    const auto& functions = table[os.opcode];
    
    /* search for perfect match first */
    auto it = std::find_if(functions.begin(), functions.end(), [&os] (const auto& fun) { return os.args == fun.first.args; });
    
    /* otherwise search for compatible overloads */
    if (it == functions.end())
    {
      std::vector<VariantFunction*> compatibles;
      for (const auto& fun : functions)
        if (os.args < fun.first.args)
        {
          if (matching) assert(false);
          else
          {
            matching = &fun.second;
            break;
          }
        }
    }
    else
      matching = &it->second;
    
    if (matching)
    {
      if (cache.find(os) != cache.end())
        assert(false);
      
      cache[os] = matching;
      return matching;
    }
    else
      return nullptr;
  }
  
  bool isAlreadyMapped(const Signature& signature)
  {
    auto oit = table.find(signature.opcode);
    if (oit != table.end())
    {
      auto it = std::find_if(oit->second.begin(), oit->second.end(), [&signature] (const auto& entry) { return entry.first == signature; });
      return it != oit->second.end();
    }
    
    return false;
  }
  
  void emplace(const Signature& signature, VariantFunction&& function)
  {
    assert(!isAlreadyMapped(signature));
    table[signature.opcode].push_back(std::make_pair(signature, function));
  }
  
  Vocabulary _vocabulary;
  
public:
  MicroCode()
  {
    static_assert(sizeof(ActualSignature) == sizeof(u64), "");
  }
  
  const Vocabulary& vocabulary() { return _vocabulary; }
  
  void registerNullary(Signature signature, SignatureArguments retn, const decltype(VariantFunction::nullary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasNullary = true;
  }
  
  void registerUnary(Signature signature, SignatureArguments retn, const decltype(VariantFunction::unary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasUnary = true;
  }
  
  void registerBinary(Signature signature, SignatureArguments retn, const decltype(VariantFunction::binary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasBinary = true;
  }
  
  void registerTernary(Signature signature, SignatureArguments retn, const decltype(VariantFunction::ternary)&& function)
  {
    emplace(signature, VariantFunction(function));
    _vocabulary.add(signature,retn);
    opcodeData[signature.opcode].hasTernary = true;
  }
  
  template<typename T, typename U, typename R, template<typename TT, typename UU, typename RR> class F> void registerNumericTemplate(Opcode opcode)
  {
    registerBinary({ opcode, type_of<T>::value, type_of<U>::value }, { type_of<R>::value }, [] (VM* vm, const Value& v1, const Value& v2) { vm->push(F<T, U, R>()(v1.number<T>(), v2.number<U>())); });
  }
  
  template<typename T1, bool IS_COMPARISON> using return_type =
    typename std::conditional<
      IS_COMPARISON,
      bool,
      T1
    >::type;
  
  template<bool IS_COMPARISON, template<typename TT, typename UU, typename RR> class OP> void registerNumeric(Opcode opcode)
  {
    registerNumericTemplate<real_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, real_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<real_t, integral_t, return_type<real_t, IS_COMPARISON>, OP>(opcode);
    registerNumericTemplate<integral_t, integral_t, return_type<integral_t, IS_COMPARISON>, OP>(opcode);
  }
  
  bool execute(VM* vm, Opcode opcode) const;
  
  const MicroCode& defaultCode();
};
