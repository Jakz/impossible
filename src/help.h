/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#ifndef _HELP_H_
#define _HELP_H_

#include "defines.h"
#include "value.h"
#include "collection.h"
#include "semantics.h"

#include <map>
#include <set>
#include <string>
#include <vector>

enum class Topic
{
  NUMERICS,
  BITWISE,
  COLLECTIONS,
  STACK,
  FUNCTIONAL,
  UTILITY,
  LOGIC,
  TEXT
};

struct TypeConstructor
{
  std::string code;
  std::string desc;
  
  static TypeConstructor make(std::string code, std::string desc)
  {
    TypeConstructor c;
    c.code = code;
    c.desc = desc;
    return c;
  }
};

struct OpHelpEntry
{
  using examples_t = std::vector<std::pair<std::string, std::string>>;
  using string = std::string;
  
  Arguments i;
  Arguments o;

  string desc;
  string ident;
  Topic topic;
  examples_t examples;
  
  OpHelpEntry() { }
  OpHelpEntry(const std::string& name, Topic topic, const Arguments& input, const Arguments& output, const std::string& desc, const examples_t& examples) :
  i(input), o(output), ident(name), desc(desc), topic(topic), examples(examples) { }
  
  static OpHelpEntry unary(Type i1, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o;
    o.i[0] = i1;
    
    o.ident = ident;
    o.topic = topic;
    o.desc = desc;

    return o;
  }
    
  static OpHelpEntry unary(Type i1, Type o1, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o = unary(i1, ident, topic, desc, example, example2);
    o.o[0] = o1;

    return o;
  }
  
  static OpHelpEntry nullaryO(Type o1, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o;
    o.i[0] = TYPE_NIL;
    o.o[0] = o1;
    
    o.ident = ident;
    o.topic = topic;
    o.desc = desc;
    
    return o;
  }
  
  static OpHelpEntry binary(Type i1, Type i2, Type o1, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o;
    o.i[0] = i1;
    o.i[1] = i2;
    o.o[0] = o1;
    
    o.ident = ident;
    o.topic = topic;
    o.desc = desc;
    
    return o;
  }
  
  static OpHelpEntry binaryO(Type i1, Type i2, Type o1, Type o2, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o = binary(i1, i2, o1, ident, topic, desc, example, example2);
    o.o[1] = o2;
    return o;
  }
  
  static OpHelpEntry ternary(Type i1, Type i2, Type i3, Type o1, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o;
    o.i[0] = i1;
    o.i[1] = i2;
    o.i[2] = i3;
    o.o[0] = o1;
    
    o.ident = ident;
    o.topic = topic;
    o.desc = desc;
    
    return o;
  }
  
  static OpHelpEntry ternaryO2(Type i1, Type i2, Type i3, Type o1, Type o2, Type o3, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o = ternary(i1, i2, i3, o1, ident, topic, desc, example, example2);
    o.o[1] = o2;
    o.o[2] = o3;
    
    return o;
  }
  
  static OpHelpEntry unaryO(Type i1, Type o1, Type o2, string ident, Topic topic, string desc, string example, string example2)
  {
    OpHelpEntry o;
    o.i[0] = i1;
    o.o[0] = o1;
    o.o[1] = o2;
    
    o.ident = ident;
    o.topic = topic;
    o.desc = desc;
    
    return o;
  }
};

/*template<>
class tr1::hash<OpHelp>
{
  public:
  size_t operator()(const OpHelp & x) const
  {
    return tr1::hash<string>()(x.name);
  }
};

template<>
class equal_to<OpHelp>
{
  public:
  bool operator() (const OpHelp& x, const OpHelp& y) const
  {
    return equal_to<string>()(x.name,y.name);
  }
};*/

class Help
{
public:
  
private:
  static std::multimap<std::string, OpHelpEntry> operators;
  static std::multimap<Type, TypeConstructor> constructors;

  static void addConstructor(Type type, TypeConstructor constructor);
  static void addOperator(std::string op, OpHelpEntry entry);
  static void printOperator(std::string op, OpHelpEntry o);

  static const char* topicString(Topic topic);

public:
  static void init();
  static bool printHelpForOperator(std::string op);
  static bool printHelpForType(const std::string& stype);
  static bool printHelpForSearch(std::string search);
  static void printOperators();
  static void printTypes();

  static void printHelpMain();
  static void printHelpSummary();
  
  static void addOperator(Opcode opcode, const Arguments& input, const Arguments& output, Topic topic, const std::string& name, const std::string& description, const OpHelpEntry::examples_t& examples);
};

#endif
