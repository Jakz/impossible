#include "semantics.h"
#include "help.h"

#include "types/error.h"
#include "types/regexpr.h"

using V = const Value&;

void registerStackFunctions(MicroCode& mc);
void registerNumericFunctions(MicroCode& mc);
void registerStringFunctions(MicroCode& mc);

SignatureArguments normalize(const SignatureArguments& args)
{
  SignatureArguments args2 = args;
  std::for_each(args2.t.begin(), args2.t.end(), [] (SignatureType& type) {
    if (type.type() >= TYPE_FIRST_GENERIC && type.type() <= TYPE_LAST_GENERIC)
      type = TYPE_GENERIC;
  });
  
  return args2;
}



void registerUnary(MicroCode& mc, Topic topic, const std::string& name, const std::string& desc,
                   const std::vector<std::pair<std::string,std::string>>& examples,
                   Signature signature, SignatureArguments retn,
                   const decltype(VariantFunction::unary)&& functor)
{
  
  mc.registerUnary({ signature.opcode, normalize(signature.args) }, retn, std::move(functor));
  Help::addOperator(signature.opcode, signature.args, retn, topic, name, desc, examples);
}

void registerUnary(MicroCode& mc, Signature args, SignatureArguments retn, const decltype(VariantFunction::unary)&& functor)
{
  registerUnary(mc, Topic::UTILITY, "", "", {}, args, retn, std::move(functor));
}

void registerBinary(MicroCode& mc, Topic topic, const std::string& name, const std::string& desc,
                   const std::vector<std::pair<std::string,std::string>>& examples,
                   Signature signature, SignatureArguments retn,
                   const decltype(VariantFunction::binary)&& functor)
{
  
  mc.registerBinary({ signature.opcode, normalize(signature.args) }, retn, std::move(functor));
  Help::addOperator(signature.opcode, signature.args, retn, topic, name, desc, examples);
}

void registerBinary(MicroCode& mc, Signature args, SignatureArguments retn, const decltype(VariantFunction::binary)&& functor)
{
  registerBinary(mc, Topic::UTILITY, "", "", {}, args, retn, std::move(functor));
}

void registerTernary(MicroCode& mc, Topic topic, const std::string& name, const std::string& desc,
                    const std::vector<std::pair<std::string,std::string>>& examples,
                    Signature signature, SignatureArguments retn,
                    const decltype(VariantFunction::ternary)&& functor)
{
  
  mc.registerTernary({ signature.opcode, normalize(signature.args) }, retn, std::move(functor));
  Help::addOperator(signature.opcode, signature.args, retn, topic, name, desc, examples);
}

void registerFunctions(MicroCode& mc)
{
  registerBinary(mc,
                 Topic::LOGIC, "equal", "returns true if two values are equal",
                 {},
                 { OP_EQUAL, TRAIT_ANY_TYPE, TRAIT_ANY_TYPE }, { },
                 [] (VM* vm, V v1, V v2) { vm->push(v1 == v2); }
                 );
  
  //TODO: return type should be unknown
  registerUnary(mc, Topic::FUNCTIONAL, "apply", "execute lambda", {}, { OP_BANG, TYPE_LAMBDA }, { TRAIT_ANY_TYPE }, [] (VM* vm, V v) {
                  vm->execute(v.lambda()->code());
                });
  
  registerUnary(mc,
                Topic::COLLECTIONS, "size", "return size/length of the value on stack",
                {{"(1 2 3)_", "3"}, {"{}_", "0"}},
                { OP_NEG, TRAIT_COUNTABLE }, { TYPE_INT },
                [] (VM* vm, const Value& v1) {
                  vm->push(v1.collection()->size());
                }
                );
  
  registerBinary(mc,
                 Topic::COLLECTIONS, "at", "retrieves i-th element from indexable type",
                 {{"(1 2 3)1@", "2"}, {"\"hello\"0@", "'h"}},
                 { OP_AT, TRAIT_INDEXABLE, TYPE_INT }, { TRAIT_ANY_TYPE },
                 [] (VM* vm, const Value& v1, const Value& v2) { vm->push(v1.indexable()->at(v2.integral())); }
                 );
  
  registerBinary(mc,
                 Topic::COLLECTIONS, "contains", "return true if value is contained in lookupable value",
                 {},
                 { OP_QUESTION, TRAIT_LOOKUPABLE, TRAIT_ANY_TYPE }, { TYPE_BOOL },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   Traits::Lookupable* lookupable = v1.object<Traits::Lookupable>();
                   vm->push(lookupable->find(v2).first);
                 });
  
  
  /* Functional applications on collections */
  registerBinary(mc,
                 Topic::COLLECTIONS, "each", "execute the lambda for each element in the iterable value",
                 {},
                 { OP_ITER, TRAIT_ITERABLE, TYPE_LAMBDA }, { },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   Iterator it = v1.object<Traits::Iterable>()->iterator();
                   while (it)
                   {
                     vm->push(*it);
                     vm->execute(v2.lambda()->code());
                     ++it;
                   }
                 }
                 );
  
  registerBinary(mc,
                 Topic::COLLECTIONS, "map", "map each value of input into a new value and store it into compatible appendable",
                 {},
                 { OP_MAP, TRAIT_ITERABLE, TYPE_LAMBDA }, { TRAIT_APPENDABLE },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   Iterator it = v1.object<Traits::Iterable>()->iterator();
                   Traits::Countable* asCountable = v1.object<Traits::Countable>();
                   auto destination = v1.type.traits().to_collector(asCountable ? asCountable->size() : 0);

                   //TODO: we're assuming that an appendable type has a to_collector function
                   if (destination.first != TYPE_NONE)
                   {
                     while (it)
                     {
                       vm->push(*it);
                       vm->execute(v2.lambda()->code());
                       
                       if (vm->stackSize() < 1)
                       {
                         vm->push(new Error(ErrorCode::OPERAND_REQUIRED_ON_STACK, "lambda supplied to map must produce a value"));
                         return;
                       }
                       
                       destination.second->put(vm->pop());
                       ++it;
                     }
                     
                     vm->push(Value(destination.first, dynamic_cast<managed_object*>(destination.second)));
                   }
                 }
                 );
  
  registerUnary(mc, Topic::COLLECTIONS, "rand-element", "get a random element from iterable", {},
                { OP_RAND, TRAIT_ITERABLE|TRAIT_COUNTABLE }, { TRAIT_ANY_TYPE },
                [] (VM* vm, V v) {
                  Traits::Indexable* indexable = v.indexable();
                  Traits::Countable* countable = v.object<Traits::Countable>();
                  integral_t index = Util::randr<integral_t>(0, countable->size());
                  
                  /* optimized for random access */
                  if (indexable)
                    vm->push(indexable->at(index));
                  else
                  {
                    auto it = v.iterable()->iterator();
                    for (integral_t i = 0; 0 < index; ++i, ++it);
                    vm->push(*it);
                  }
                });
  
  
  registerBinary(mc,
                 Topic::COLLECTIONS, "filter", "filter all values which match a predicate into a new collection",
                 {},
                 { OP_RSHIFT, TRAIT_ITERABLE, TYPE_LAMBDA }, { TRAIT_APPENDABLE },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   Iterator it = v1.object<Traits::Iterable>()->iterator();
                   
                   auto destination = v1.type.traits().to_collector(0);
                   
                   //TODO: we're assuming that an appendable type has a to_collector function
                   if (destination.first != TYPE_NONE)
                   {
                     while (it)
                     {
                       vm->push(*it);
                       vm->execute(v2.lambda()->code());

                       Value v = vm->pop();
                       if (v.type.traits().to_bool(v))
                       {
                         destination.second->put(*it);
                       }
                       ++it;
                     }
                     
                     vm->push(Value(destination.first, dynamic_cast<managed_object*>(destination.second)));
                   }
                   
                 });
  
  registerTernary(mc,
                 Topic::COLLECTIONS, "cartesian", "apply lambda to each possible pair of two iterable values",
                 {},
                 { OP_CARTESIAN, TRAIT_ITERABLE, TRAIT_ITERABLE, TYPE_LAMBDA }, { TRAIT_APPENDABLE },
                 [] (VM* vm, V v1, V v2, V v3) {
                   Code* code = v3.lambda()->code();
                   
                   std::function<integral_t(Traits::Countable*)> counter = [] (Traits::Countable* ctb) { return ctb->size(); };
                   integral_t sizeHint1 = v1.traitOrElse<>(counter, 0LL);
                   integral_t sizeHint2 = v1.traitOrElse<>(counter, 0LL);
                   
                   auto dest = v1.type.traits().to_collector(sizeHint1 + sizeHint2);
                   
                   Iterator it1 = v1.iterable()->iterator();
                   while (it1)
                   {
                     Iterator it2 = v2.iterable()->iterator();

                     while (it2)
                     {
                       vm->push(*it1);
                       vm->push(*it2);
                       vm->execute(code);
                       //TODO: check value is present
                       dest.second->put(vm->pop());
                       
                       ++it2;
                     }
                     
                     ++it1;
                   }
                   
                   vm->push(Value(dest.first, dynamic_cast<managed_object*>(dest.second)));
                   
                 });
  
  auto fold = [] (VM* vm, V v1, Code* code, bool hasInit)
  {
    Iterator it = v1.iterable()->iterator();
    
    //TODO: v1 requires at lease 1/2 elements according to hasInit
    
    if (!hasInit)
    {
      if (!it)
      {
        vm->push(Value());
        return;
      }
      
      vm->push(*it);
      ++it;
    }
    
    while (it)
    {
      vm->push(*it);
      vm->execute(code);
      /* next element should be already on stack */
      ++it;
    }
  };
  
  registerTernary(mc, Topic::COLLECTIONS, "fold", "folds an iterable", {},
                  { OP_FOLD, TRAIT_ITERABLE, TYPE_LAMBDA, TRAIT_ANY_TYPE }, { TRAIT_ANY_TYPE },
                  [&fold] (VM* vm, V v1, V v2, V v3) {
                    vm->push(v3);
                    fold(vm, v1, v2.lambda()->code(), true);
                  });
  
  registerBinary(mc, Topic::COLLECTIONS, "any", "check if any element of iterable satisfies a lambda", {},
                  { OP_ANY, TRAIT_ITERABLE, TYPE_LAMBDA }, { TYPE_BOOL },
                  [] (VM* vm, V v1, V v2) {
                    Code* code = v2.lambda()->code();
                    Traits::Iterable* iterable = v1.iterable();
                    auto it = iterable->iterator();
                    while (it)
                    {
                      vm->push(*it);
                      vm->execute(code);
                      //TODO: check value on stack and type?
                      Value v = vm->pop();
                      if (v.type.traits().to_bool(v))
                      {
                        vm->push(true);
                        return;
                      }
                      ++it;
                    }
                    
                    vm->push(false);
                  });
  
  registerBinary(mc, Topic::COLLECTIONS, "every", "check if all elements of iterable satisfy a lambda", {},
                 { OP_EVERY, TRAIT_ITERABLE, TYPE_LAMBDA }, { TYPE_BOOL },
                 [] (VM* vm, V v1, V v2) {
                   Code* code = v2.lambda()->code();
                   Traits::Iterable* iterable = v1.iterable();
                   auto it = iterable->iterator();
                   while (it)
                   {
                     vm->push(*it);
                     vm->execute(code);
                     //TODO: check value on stack and type?
                     Value v = vm->pop();
                     if (!v.type.traits().to_bool(v))
                     {
                       vm->push(false);
                       return;
                     }
                     ++it;
                   }
                   
                   vm->push(true);
                 });
  
  
  std::initializer_list<std::tuple<Opcode, Opcode, Opcode, std::string>> embedded_helpers = {
    std::make_tuple( OP_PLUS_DIA, OP_PLUS_MON, OP_PLUS, "sum" ),
    std::make_tuple( OP_MINUS_DIA, OP_MINUS_MON, OP_MINUS, "subtract" ),
    std::make_tuple( OP_TIMES_DIA, OP_TIMES_MON, OP_TIMES, "multiply" ),
    std::make_tuple( OP_DIVIDE_DIA, OP_DIVIDE_MON, OP_DIVIDE, "divide" )
  };
  
  
  //TODO: add support for default values if iterable have different lengths
  auto double_map = [] (VM* vm, V v1, V v2, Code* code)
  {
    std::function<integral_t(Traits::Countable*)> counter = [] (Traits::Countable* ctb) { return ctb->size(); };
    integral_t sizeHint1 = v1.traitOrElse<>(counter, 0LL);
    integral_t sizeHint2 = v1.traitOrElse<>(counter, 0LL);
    
    auto dest = v1.type.traits().to_collector(std::min(sizeHint1, sizeHint2));
    
    Iterator it1 = v1.iterable()->iterator();
    Iterator it2 = v2.iterable()->iterator();
    
    while (it1 || it2)
    {
      vm->push(*it1);
      vm->push(*it2);
      vm->execute(code);
      dest.second->put(vm->pop());
      
      ++it1;
      ++it2;
    }
    
    vm->push(Value(dest.first, dynamic_cast<managed_object*>(dest.second)));
  };
  
  registerTernary(mc,
                  Topic::COLLECTIONS, "dmap", "apply lambda to each consecutive pair of two iterable values",
                  {},
                  { OP_DMAP, TRAIT_ITERABLE, TRAIT_ITERABLE, TYPE_LAMBDA }, { TRAIT_APPENDABLE },
                  [&double_map] (VM* vm, V v1, V v2, V v3) { double_map(vm, v1, v2, v3.lambda()->code()); });
  
  for (const auto& entry : embedded_helpers)
  {
    registerBinary(mc, Topic::COLLECTIONS,
                   std::string("diadic-") + std::get<3>(entry), std::string(std::get<3>(entry)) + " each consecutive pair of two iterable values",
                   {},
                   { std::get<0>(entry), TRAIT_ITERABLE, TRAIT_ITERABLE }, { TRAIT_APPENDABLE },
                   [&double_map, entry] (VM* vm, V v1, V v2) { //TODO: entry passed by value
                     CodeStandard code = CodeStandard(Instruction(std::get<2>(entry)));
                     double_map(vm, v1, v2, &code);
                   });
    
    registerUnary(mc, Topic::COLLECTIONS,
                   std::string("monadic-") + std::get<3>(entry), std::string(std::get<3>(entry)) + " each value contained inside iterable value",
                   {},
                   { std::get<1>(entry), TRAIT_ITERABLE }, { TRAIT_ANY_TYPE },
                   [&fold, entry] (VM* vm, V v1) { //TODO: entry passed by value
                     CodeStandard code = CodeStandard(Instruction(std::get<2>(entry)));
                     fold(vm, v1, &code, false);
                   });
  }

  //TODO: should return a String if input is a String
  registerBinary(mc,
                 Topic::COLLECTIONS, "extract", "retrieve all elements of range from indexable type",
                 {},
                 { OP_AT, TRAIT_INDEXABLE | TRAIT_COUNTABLE, TYPE_RANGE }, { TYPE_ARRAY },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   const RangeVector& r = v2.range()->raw();
                   Array::utype_t nv;
                   Traits::Indexable* source = v1.object<Traits::Indexable>();
                   integral_t size = v1.object<Traits::Countable>()->size();
                   
                   std::vector<integral_t> iv = r.concretize();
                   nv.reserve(iv.size());
                   
                   for (size_t i = 0; i < iv.size(); ++i)
                   {
                     integral_t j = iv[i];
                     if (j < 0) j += size;
                     if (j < size) nv.push_back(source->at(j));
                   }
                   
                   vm->push(v1);
                   vm->push(new Array(nv));
                 });
  
  registerUnary(mc, Topic::COLLECTIONS, "min", "find minimum value in iterable", {},
                {OP_LESSER, TRAIT_ITERABLE }, { TRAIT_ANY_TYPE }, [] (VM* vm, V v1) {
                  Traits::Iterable* iterable = v1.iterable();
                  Iterator it = iterable->iterator();
                  
                  if (!it)
                    vm->push(Value());
                  else
                  {
                    Value v = *it;
                    ++it;
                    while (it)
                    {
                      v = std::min(v, *it);
                      ++it;
                    }
                    vm->push(v);
                  }
                });
  
  registerUnary(mc, Topic::COLLECTIONS, "max", "find maximum value in iterable", {},
                {OP_GREATER, TRAIT_ITERABLE }, { TRAIT_ANY_TYPE }, [] (VM* vm, V v1) {
                  Traits::Iterable* iterable = v1.iterable();
                  Iterator it = iterable->iterator();
                  
                  if (!it)
                    vm->push(Value());
                  else
                  {
                    Value v = *it;
                    ++it;
                    while (it)
                    {
                      v = std::max(v, *it);
                      ++it;
                    }
                    vm->push(v);
                  }
                });
  
  /* set related */
  registerBinary(mc,
                 Topic::COLLECTIONS, "intersection", "computes intersection between two sets",
                 {},
                 { OP_AND, TYPE_SET, TYPE_SET }, { TYPE_SET },
                 [] (VM* vm, V v1, V v2) {
                   Set::data_t result;
                   for (const auto& v : v1.set()->raw())
                     if (v2.set()->raw().find(v) != v2.set()->raw().end())
                       result.insert(v);
                   
                   vm->push(new Set(result));
                 
                 });
  
  registerBinary(mc,
                 Topic::COLLECTIONS, "union", "computes union between two sets",
                 {},
                 { OP_OR, TYPE_SET, TYPE_SET }, { TYPE_SET },
                 [] (VM* vm, V v1, V v2) {
                   Set::data_t result;
                   result.insert(v1.set()->raw().begin(), v1.set()->raw().end());
                   result.insert(v2.set()->raw().begin(), v2.set()->raw().end());
                   vm->push(new Set(result));                   
                 });
  
  /* fetch from tuple */
  registerUnary(mc, {OP_ANY, TYPE_TUPLE}, { TRAIT_ANY_TYPE }, [](VM* vm, V v1) { vm->push(v1.tuple()->at(0)); });
  registerUnary(mc, {OP_EVERY, TYPE_TUPLE}, { TRAIT_ANY_TYPE }, [](VM* vm, V v1) { vm->push(v1.tuple()->at(1)); });

  

  registerStackFunctions(mc);
  registerNumericFunctions(mc);
  registerStringFunctions(mc);
}

namespace math
{
  template<typename T, typename U, typename R> struct plus { public: R operator()(T t, U u) { return t + u;} };
  template<typename T, typename U, typename R> struct minus { public: R operator()(T t, U u) { return t - u;} };
  template<typename T, typename U, typename R> struct times { public: R operator()(T t, U u) { return t * u;} };
  template<typename T, typename U, typename R> struct divide { public: R operator()(T t, U u) { return t / u;} };
  
  template<typename T, typename U, typename R> struct lesser { public: R operator()(T t, U u) { return t < u;} };
  template<typename T, typename U, typename R> struct greater { public: R operator()(T t, U u) { return t > u;} };
  
  template<typename T, typename U, typename R> struct min { public: R operator()(T t, U u) { return t < u ? t : u; } };
  template<typename T, typename U, typename R> struct max { public: R operator()(T t, U u) { return t > u ? t : u; } };

}

#pragma mark Numeric Functions
void registerNumericFunctions(MicroCode& mc)
{
  mc.registerNumeric<false, math::plus>(OP_PLUS);
  mc.registerNumeric<false, math::minus>(OP_MINUS);
  mc.registerNumeric<false, math::times>(OP_TIMES);
  mc.registerNumeric<false, math::divide>(OP_DIVIDE);
  
  mc.registerNumeric<true, math::lesser>(OP_LESSER);
  mc.registerNumeric<true, math::greater>(OP_GREATER);
  
  mc.registerNumeric<false, math::min>(OP_ANY);
  mc.registerNumeric<false, math::max>(OP_EVERY);

  registerUnary(mc, {OP_NEG, TYPE_INT }, { TYPE_INT }, [](VM* vm, V v) { vm->push(-v.integral()); });
  registerUnary(mc, {OP_NEG, TYPE_FLOAT }, { TYPE_FLOAT }, [](VM* vm, V v) { vm->push(-v.real()); });

  registerUnary(mc, {OP_NOT, TYPE_FLOAT }, { TYPE_FLOAT }, [](VM* vm, V v) { vm->push(1.0 / v.real()); });
  
  registerBinary(mc, {OP_MOD, TYPE_INT, TYPE_INT }, { TYPE_INT }, [](VM* vm, V v1, V v2) { vm->push(v1.integral() % v2.integral()); });
  registerUnary(mc, {OP_MOD, TYPE_FLOAT }, { TYPE_TUPLE }, [](VM* vm, V v) {
    real_t i, f;
    f = modf(v.real(), &i);
    vm->push(new Tuple(i, f));
  });
  
  registerUnary(mc, {OP_BANG, TYPE_INT}, { TYPE_INT }, [](VM* vm, V vv) {
    integral_t res = 1;
    integral_t v = vv.integral();
    while (v > 1) res *= v--;
    vm->push(res);
  });

  
  auto numberIsInRange = [](VM* vm, V v1, V v2) {
    const RangeVector& range = v2.range()->raw();
    numeric_t v = v1.number();

    bool r = std::any_of(range.begin(), range.end(), [&v] (const auto& pair) { return v >= pair.a && v <= pair.b; });
    vm->push(r);
  };
  
  registerBinary(mc, {OP_LESSER, TYPE_INT, TYPE_RANGE}, { TYPE_BOOL }, numberIsInRange);
  registerBinary(mc, {OP_LESSER, TYPE_FLOAT, TYPE_RANGE}, { TYPE_BOOL }, numberIsInRange);

  
  /* bitwise */
  registerBinary(mc, Topic::BITWISE, "and", "bitwise and", {}, {OP_AND, TYPE_INT, TYPE_INT}, { TYPE_INT },
                 [](VM* vm, V v1, V v2) { vm->push(std::bit_and<>()(v1.integral(), v2.integral())); }
                 );
  registerBinary(mc, Topic::BITWISE, "or", "bitwise or", {}, {OP_OR, TYPE_INT, TYPE_INT}, { TYPE_INT },
                 [](VM* vm, V v1, V v2) { vm->push(std::bit_or<>()(v1.integral(), v2.integral())); }
                 );
  registerBinary(mc, Topic::BITWISE, "left-shift", "bitwise left shift", {},
                 {OP_LSHIFT, TYPE_INT, TYPE_INT}, { TYPE_INT }, [](VM* vm, V v1, V v2) { vm->push(v1.integral() << v2.integral()); }
                 );
  registerBinary(mc, Topic::BITWISE, "right-shift", "bitwise right shift", {}, {OP_RSHIFT, TYPE_INT, TYPE_INT}, { TYPE_INT },
                 [](VM* vm, V v1, V v2) { vm->push(v1.integral() >> v2.integral()); }
                 );
  registerUnary(mc, {OP_NOT, TYPE_INT }, { TYPE_INT }, [](VM* vm, V v) { vm->push(~v.integral()); });
  
  /* logical */
  registerBinary(mc, {OP_AND, TYPE_BOOL, TYPE_BOOL}, { TYPE_BOOL }, [](VM* vm, V v1, V v2) { vm->push(std::logical_and<>()(v1.boolean(), v2.boolean())); });
  registerBinary(mc, {OP_OR, TYPE_BOOL, TYPE_BOOL}, { TYPE_BOOL }, [](VM* vm, V v1, V v2) { vm->push(std::logical_or<>()(v1.boolean(), v2.boolean())); });
  registerUnary(mc, {OP_NOT, TYPE_BOOL }, { TYPE_BOOL }, [](VM* vm, V v) { vm->push(std::logical_not<>()(v.boolean())); });

}


#pragma mark Stack Functions
void registerStackFunctions(MicroCode& mc)
{
  registerUnary(mc,
                Topic::STACK, "dupe", "duplicate topmost stack value",
                {{"(1 2)$", "(1 2)(1 2)"}, {"tf$", "tff"}},
                { OP_DUPE, TRAIT_ANY_TYPE }, { TRAIT_ANY_TYPE, TRAIT_ANY_TYPE },
                [] (VM* vm, const Value& v1) { vm->push(v1); vm->push(v1); }
                );
  
  registerBinary(mc,
                 Topic::STACK, "swap", "swap two topmost stack values",
                 {{"1 2%", "2 1"}},
                 { OP_SWAP, TRAIT_ANY_TYPE, TRAIT_ANY_TYPE2 }, { TRAIT_ANY_TYPE2, TRAIT_ANY_TYPE },
                 [] (VM* vm, const Value& v1, const Value& v2) { vm->push(v2); vm->push(v1); }
                 );
  
  registerUnary(mc,
                 Topic::STACK, "pick", "copy i-th value from stack to top",
                 {},
                 { OP_PICK, TYPE_INT }, { TRAIT_ANY_TYPE },
                 [] (VM* vm, const Value& v) {
                   integral_t i = v.integral();
                   vm->push(vm->pick(i));
                 });
  
  registerUnary(mc,
                Topic::STACK, "drop", "drop topmost stack value",
                {},
                { OP_DROP, TRAIT_ANY_TYPE }, { },
                [] (VM* vm, const Value& v) { }
                );
  
  //TODO: check return signature for rise and sink
  registerTernary(mc,
                  Topic::STACK, "rise", "cycle top three stack element left",
                  {},
                  { OP_RISE, TRAIT_ANY_TYPE, TRAIT_ANY_TYPE2, TRAIT_ANY_TYPE3 }, { TRAIT_ANY_TYPE2, TRAIT_ANY_TYPE3, TRAIT_ANY_TYPE },
                  [] (VM* vm, V v1, V v2, V v3) {
                    vm->push(v2);
                    vm->push(v3);
                    vm->push(v1);
                  });
  
  registerTernary(mc,
                  Topic::STACK, "sink", "cycle top three stack element right",
                  {},
                  { OP_SINK, TRAIT_ANY_TYPE, TRAIT_ANY_TYPE2, TRAIT_ANY_TYPE3 }, { TRAIT_ANY_TYPE3, TRAIT_ANY_TYPE2, TRAIT_ANY_TYPE },
                  [] (VM* vm, V v1, V v2, V v3) {
                    vm->push(v3);
                    vm->push(v1);
                    vm->push(v2);
                  });
}

void registerStringFunctions(MicroCode& mc)
{
  registerUnary(mc,
                 Topic::TEXT, "compile-regex", "compile a regex from a string",
                 {},
                 { OP_DIVIDE, TYPE_STRING }, { TYPE_REGEX },
                 [] (VM* vm, const Value& v)
                 {
                   Regex* regex = nullptr;
                   try
                   {
                     regex = new Regex(v.string()->raw());
                     vm->push(regex);
                   }
                   catch (std::regex_error e)
                   {
                     vm->push(new Error(ErrorCode::INVALID_REGEX, e.what()));
                   }
                 });
  
  registerBinary(mc, Topic::TEXT, "partial-match", "verify partial match between a regex and a string", {}, { OP_ANY, TYPE_REGEX, TYPE_STRING }, { TYPE_BOOL },
                 [] (VM* vm, const Value& v1, const Value& v2)
                 {
                   Regex* regex = v1.regex();
                   String* string = v2.string();
                   vm->push(std::regex_search(string->raw(), regex->raw()));
                 });
  
  registerBinary(mc, Topic::TEXT, "full-match", "verify full match between a regex and a string", {}, { OP_EVERY, TYPE_REGEX, TYPE_STRING }, { TYPE_BOOL },
                 [] (VM* vm, const Value& v1, const Value& v2)
                 {
                   Regex* regex = v1.regex();
                   String* string = v2.string();
                   vm->push(std::regex_match(string->raw(), regex->raw()));
                 });
}

