//
//  CompositeTypes.cpp
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#include "collection.h"
#include "types/traits.h"
#include "instruction.h"

#include <iostream>
#include <iomanip>
#include <sstream>

void Lambda::put(size_t index, Value value)
{
  index = std::min(index, _code->size());
}

integral_t Lambda::size() const { return _code->size(); }

//TODO: MOVE!

std::mt19937_64& Util::rnd()
{
  static std::random_device r;
  static std::default_random_engine er(r());
  static std::mt19937_64 _rnd(er());
  
  return _rnd;
}
