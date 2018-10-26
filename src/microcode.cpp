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
                Topic::COLLECTIONS, "size", "returns size of the collection",
                {{"(1 2 3)_", "3"}, {"{}_", "0"}},
                { OP_NEG, TYPE_COLLECTION }, { TYPE_INT },
                [] (VM* vm, const Value& v1) {
                  vm->push(v1.collection()->size());
                }
                );
  
  /** STACK FUNCTIONS **/
  registerUnary(mc,
                Topic::STACK, "dupe", "duplicates topmost stack value",
                {{"(1 2)$", "(1 2)(1 2)"}, {"tf$", "tff"}},
                { OP_DUPE, TYPE_GENERIC }, { TYPE_GENERIC, TYPE_GENERIC },
                [] (VM* vm, const Value& v1) { vm->push(v1); vm->push(v1); }
                );
  
  registerBinary(mc,
                Topic::STACK, "swap", "swap two topmost stack values",
                {{"1 2%", "2 1"}},
                { OP_SWAP, TYPE_GENERIC, TYPE_GENERIC2 }, { TYPE_GENERIC, TYPE_GENERIC2 },
                [] (VM* vm, const Value& v1, const Value& v2) { vm->push(v2); vm->push(v1); }
                );
}

