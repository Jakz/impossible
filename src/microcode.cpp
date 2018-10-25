#include "semantics.h"
#include "help.h"


void registerUnary(MicroCode& mc, Topic topic, const std::string& name, const std::string& desc,
                   const std::vector<std::pair<std::string,std::string>>& examples,
                   Signature signature, Arguments retn,
                   const decltype(VariantFunction::unary)&& functor)
{
  
  mc.registerUnary(signature, retn, std::move(functor));
  Help::addOperator(signature.opcode, signature.args, retn, topic, name, desc, examples);
}

void registerFunctions(MicroCode& mc)
{
  mc.registerDefault();
  
  registerUnary(mc,
                Topic::COLLECTIONS, "size", "returns size of the collection",
                {{"(1 2 3)_", "3"}, {"{}_", "0"}},
                { OP_NEG, TYPE_COLLECTION }, TYPE_INT,
                [] (VM* vm, const Value& v1) {
                  vm->push(v1.collection()->size());
                }
                );
}

