/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdlib.h>
#include <algorithm>

using s16 = int16_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s64 = uint64_t;
using u64 = uint64_t;

using integral = s64;

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

enum Type : s16
{
  TYPE_INVALID = -1,
  
  TYPE_NIL = 0,
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
  TYPE_GENERIC,
  TYPE_COLLECTION,
  
  TYPES_COUNT,
  
  TYPE_GENERIC2 = 500,
  TYPE_GENERIC3,
  TYPE_UNKNOWN,
  
  TYPE_NONE
};

#endif

// EULER 1    1..1000~[$5//0=%3//0=|]>>+.
// EULER 2    (?0:[1]1:[1][1<>2<>+])[4kk>~]<>~[2//0=]>>+.
// EULER 4    1..999$[*]:*.[s$~=]>> >

