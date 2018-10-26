#include "semantics.h"
#include "help.h"

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


void registerFunctions(MicroCode& mc)
{
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
  

}

