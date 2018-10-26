/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#pragma once

/**************************
 * CONFIG
 **************************/

#define USE_PACKED_VALUES false

/**************************/

#include <stdlib.h>
#include <algorithm>

using s16 = int16_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s64 = int64_t;
using u64 = uint64_t;

using integral_t = s64;
using real_t = double;

struct managed_object
{
  virtual ~managed_object() { }
};

struct enum_hash
{
  template <typename T>
  inline
  typename std::enable_if<std::is_enum<T>::value, size_t>::type
  operator ()(T const value) const
  {
    return static_cast<size_t>(static_cast<size_t>(value));
  }
};

template <bool B, typename T, T trueval, T falseval>
struct conditional_value : std::conditional<B, std::integral_constant<T, trueval>, std::integral_constant<T, falseval>>::type { };

class Util
{
  public:
    static inline int randi(int min, int max) { if (max==min) return min; return rand()%(max-min) + min; }
  
};

enum Opcode : u16
{
  OP_PLUS = 0,
  OP_MINUS,
  OP_TIMES,
  OP_DIVIDE,
  
  OP_MOD,
  OP_NEG,
  
  OP_AND,
  OP_OR,
  OP_NOT,
  OP_RSHIFT,
  OP_LSHIFT,
  
  OP_PLUS_MON,
  OP_PLUS_DIA,
  OP_MINUS_MON,
  OP_MINUS_DIA,
  OP_TIMES_MON,
  OP_TIMES_DIA,
  OP_DIVIDE_MON,
  OP_DIVIDE_DIA,
  
  OP_BANG,
  OP_DBANG,
  OP_QUESTION,
  OP_DQUESTION,
  
  OP_DUPE,
  OP_SWAP,
  OP_DROP,
  OP_PICK,
  OP_RISE,
  OP_SINK,
  
  OP_EQUAL,
  OP_LESSER,
  OP_GREATER,
  
  OP_PEEK,
  
  OP_RAND,
  OP_SORT,
  OP_SHUFFLE,
  
  OP_AT,
  OP_HASH,
  OP_AT_FRONT,
  OP_AT_BACK,
  OP_HASH_FRONT,
  OP_HASH_BACK,
  
  OP_ITER,
  OP_ITERI,
  OP_MAP,
  OP_FOLD,
  OP_DMAP,
  OP_CARTESIAN,
  
  OP_ANY,
  OP_EVERY,
  
  OP_RECUR,
  OP_RECURV,
  OP_WHILE,
  
  OP_RAISE_STACK,
  OP_LOWER_STACK,
  OP_RAISE_STACKV,
  OP_LOWER_STACKV,
  
  OP_STRING_CAST,
  
  
  
  OPCODES_COUNT
};

enum Type : u16
{
  TYPE_NIL = 0,
  TYPE_OPCODE,
  TYPE_INT,
  TYPE_FLOAT,
  
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_RANGE,
  
  TYPE_LIST,
  TYPE_ARRAY,
  TYPE_SET,
  TYPE_MAP,
  TYPE_STACK,
  TYPE_QUEUE,
  TYPE_LAZY_ARRAY,
  
  TYPE_LAMBDA,
  
  TYPE_COLLECTION,

  TYPE_GENERIC = 256,
  TYPE_GENERIC2,
  TYPE_GENERIC3,

  TYPE_FIRST_GENERIC = TYPE_GENERIC,
  TYPE_LAST_GENERIC = TYPE_GENERIC3,

  TYPE_UNKNOWN = 512,
  TYPE_INVALID,
  TYPE_NONE,

  
  TYPE_MASK = 0xFFFF
};

enum Trait : u32
{  
  TRAIT_SPECIFIC_TYPE = 0x80000000,
  TRAIT_ANY_TYPE      = 0x41000000,
  TRAIT_ANY_TYPE2     = TRAIT_ANY_TYPE + 1,
  TRAIT_ANY_TYPE_LAST = 0x41000009,
  
  TRAIT_COUNTABLE               = 0x00000001,
  TRAIT_INDEXABLE               = 0x00000002,
  
  TRAIT_MAX = 8
};

inline bool operator&&(Trait t1, Trait t2) { return (static_cast<std::underlying_type<Trait>::type>(t1) & static_cast<std::underlying_type<Trait>::type>(t2)) != 0; }
inline Trait operator|(Trait t1, Trait t2) { return static_cast<Trait>((u32)t1 | (u32)t2); }

#include <string>
#include <functional>
#include <sstream>

template<typename T>
class string_joiner
{
  std::string prefix;
  std::string suffix;
  std::string separator;
  std::function<std::string(const T&)> mapper;
  std::function<bool(const T&)> skipper;
  
public:
  string_joiner(
                const std::string& prefix,
                const std::string& suffix,
                const std::string& separator,
                const std::function<std::string(const T&)>& mapper,
                const std::function<bool(const T&)>& skipper = [](const T&) { return false; }
                )
  : prefix(prefix), suffix(suffix), separator(separator), mapper(mapper), skipper(skipper) { }
  
  template<typename U> std::string join(const U& data)
  {
    std::stringstream ss(std::stringstream::out);
    ss << prefix; bool first = true;
    for (const auto& v : data)
    {
      if (skipper(v))
        continue;
      else if (first)
        first = false;
      else
        ss << separator;
    
      ss << mapper(v);
    }
    
    ss << suffix;
    
    return ss.str();
  }
};


// EULER 1    1..1000~[$5//0=%3//0=|]>>+.
// EULER 2    (?0:[1]1:[1][1<>2<>+])[4kk>~]<>~[2//0=]>>+.
// EULER 4    1..999$[*]:*.[s$~=]>> >

