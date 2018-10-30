#include "semantics.h"
#include "help.h"

#include "types/error.h"

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
  using V = const Value&;
  
  mc.registerDefault();
  
  registerUnary(mc,
                Topic::COLLECTIONS, "size", "returns size/length of the value on stack",
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
                 Topic::COLLECTIONS, "each", "executes the lambda for each element in the iterable value",
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
                 Topic::COLLECTIONS, "map", "maps each value of input into a new value and store it into compatible appendable",
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
  
  //TODO: should return a String if input is a String
  registerBinary(mc,
                 Topic::COLLECTIONS, "extract", "retrieves all elements of range from indexable type",
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
  
#pragma mark Stack Functions
  /** STACK FUNCTIONS **/
  registerUnary(mc,
                Topic::STACK, "dupe", "duplicates topmost stack value",
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
                Topic::STACK, "pick", "copies i-th value from stack to top",
                {},
                { OP_PICK, TRAIT_ANY_TYPE, TYPE_INT }, { TRAIT_ANY_TYPE },
                [] (VM* vm, const Value& v1, const Value& v2) {
                  integral_t i = v1.integral();
                  vm->push(vm->pick(i));
                });
  
  registerUnary(mc,
                 Topic::STACK, "drop", "drops topmost stack value",
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

