//
//  tests.h
//  Impossible
//
//  Created by Jack on 5/31/13.
//
//

#define _VTABLE_H_

#ifndef _VTABLE_H_
#define _VTABLE_H_

#include "vm.h"
#include "Value.h"
#include "defines.h"

template <class Value>
class type_of {
  public:
    static inline Type value() { return TYPE_NIL; }
    static inline bool isCollection() { return false; }
};

template <>
class type_of<Float> {
  public:
    static inline Type value() { return TYPE_FLOAT; }
    static inline bool isCollection() { return true; }
};

template <Type type>
class is_collection {
  public: static const bool value = false;
};

template <>
class is_collection<TYPE_FLOAT> { public: static const bool value = false; };





std::ostream& operator<<(std::ostream& stream, const Type type) {
  switch (type) {
    case TYPE_FLOAT: stream << "float"; break;
    default: stream << "nil";
  }
      
  return stream;
}
      
std::ostream& operator<<(std::ostream& stream, const Opcode type) {
  switch (type) {
    case OP_NEG: stream << "_"; break;
    default: stream << " ";
  }
  
  return stream;
}

class Operation
{
  Type i[3];
  Type o[3];
  u16 ic, oc;
  
  Opcode op;
  
public:
  Operation(Opcode op, Type i1 = TYPE_INVALID, Type i2 = TYPE_INVALID, Type i3 = TYPE_INVALID, Type o1 = TYPE_INVALID, Type o2 = TYPE_INVALID, Type o3 = TYPE_INVALID) : op{op}, i{i1,i2,i3}, o{o1,o2,o3}
  {
    ic = static_cast<u16>(count_if(i, i+3, [](Type i){return i != TYPE_INVALID;}));
    oc = static_cast<u16>(count_if(o, o+3, [](Type i){return i != TYPE_INVALID;}));

    cout << "Instruction " << op << " ic: " << ic << " oc: " << oc << endl;
  }
  
  friend class VTableMain;
};

    
class VTableNode;
class VTableMain;

enum VTableType {
  VTABLE_OPERATION = 1,
  VTABLE_TABLE = 2
};

class VTableNode
{
  Operation* operation{nullptr};
  VTableNode* table[TYPES_COUNT]{nullptr};

public:
  VTableNode() {
    operation = nullptr;
  }
  
  VTableNode(Operation *op) : operation(op) {
  }
  
  friend class VTableMain;
};

class VTableMain
{
  VTableNode **nodes = new VTableNode*[OPCODES_COUNT]{nullptr};
  
  public:
    static VTableMain *vtable;
  
    void map(Operation* operation) {
      VTableNode *node = nodes[operation->op];
      
      if (!node)
      {
        node = new VTableNode();
        nodes[operation->op] = node;
      }
      
      if (operation->ic == 0)
      {
        if (operation->i[0] != TYPE_GENERIC)
          node[operation->i[0]] = operation;
      }
    }
};





      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
template <Opcode O, class I1>
class Unary : public Operation
{
  public:
    Unary() : Operation(O, type_of<I1>::value()) { };
  
    void execute(VM *vm) {
      Value *v1;
      vm->popOne(&v1);
      innerExecute(vm, static_cast<I1*>(v1));
    }
  
    void innerExecute(VM *vm, I1 *i1) {
    
    }
};

class NegFloat : public Unary<OP_NEG, Float>
{
  void innerExecute(VM *vm, Float* i1) {
    vm->push(new Float(-i1->get()));
  }
};
      
      
      
template <Opcode O, class I1, class I2>
class Binary : public Operation
{
public:
  Binary() : Operation(O, type_of<I1>::value(), type_of<I2>::value()) { };
  
  void execute(VM *vm) {
    Value *v1, *v2;
    vm->popTwo(&v1, &v2);
    innerExecute(vm, static_cast<I1*>(v1), static_cast<I2*>(v2));
  }
  
  void innerExecute(VM *vm, I1 *i1, I2 *i2);
};
      
class PlusIntInt : public Binary<OP_PLUS, Int, Int>
{
  void innerExecute(VM *vm, Int *i1, Int *i2) {
    vm->push(new Int(i1->get()+i2->get()));
  }
};

void test()
{
  new NegFloat();
  new PlusIntInt();
}


#endif
