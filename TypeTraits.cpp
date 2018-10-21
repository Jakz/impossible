#include "TypeTraits.h"

const std::unordered_map<Type, TypeTraits::TypeSpec, enum_hash> TypeTraits::specs =
{
  { TYPE_INT, { TYPE_INT, true, false, "int" } }
};
