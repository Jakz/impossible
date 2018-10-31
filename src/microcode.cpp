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

void registerBinary(MicroCode& mc, Topic topic, const std::string& name, const std::string& desc,
                   const std::vector<std::pair<std::string,std::string>>& examples,
                   Signature signature, SignatureArguments retn,
                   const decltype(VariantFunction::binary)&& functor)
{
  
  mc.registerBinary({ signature.opcode, normalize(signature.args) }, retn, std::move(functor));
  Help::addOperator(signature.opcode, signature.args, retn, topic, name, desc, examples);
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
  
  /* bitwise */
  mc.registerBinary({OP_AND, TYPE_INT, TYPE_INT}, { TYPE_INT }, [](VM* vm, V v1, V v2) { vm->push(std::bit_and<>()(v1.integral(), v2.integral())); });
  mc.registerBinary({OP_OR, TYPE_INT, TYPE_INT}, { TYPE_INT }, [](VM* vm, V v1, V v2) { vm->push(std::bit_or<>()(v1.integral(), v2.integral())); });
  
  /* logical */
  mc.registerBinary({OP_AND, TYPE_BOOL, TYPE_BOOL}, { TYPE_BOOL }, [](VM* vm, V v1, V v2) { vm->push(std::logical_and<>()(v1.boolean(), v2.boolean())); });
  mc.registerBinary({OP_OR, TYPE_BOOL, TYPE_BOOL}, { TYPE_BOOL }, [](VM* vm, V v1, V v2) { vm->push(std::logical_or<>()(v1.boolean(), v2.boolean())); });

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
  
  registerBinary(mc,
                 Topic::STACK, "pick", "copy i-th value from stack to top",
                 {},
                 { OP_PICK, TRAIT_ANY_TYPE, TYPE_INT }, { TRAIT_ANY_TYPE },
                 [] (VM* vm, const Value& v1, const Value& v2) {
                   integral_t i = v1.integral();
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
}

