/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#include "instruction.h"

#include "vm.h"

#include <cassert>
#include <array>

static void fold(TCollection* values, Value& o, Code *code, VM *vm)
{
  vm->push(o);
  
  values->iterate();
  while (values->hasNext())
  {
    vm->push(values->next());
    vm->execute(code);
  }
  
  vm->popOne(o);
}

static void sfold(TCollection* values, Value& o, Code *code, VM *vm)
{
  values->iterate();
  
  if (values->hasNext())
  {
    Value v = values->next();
    vm->push(v);

    while (values->hasNext())
    {
      vm->push(values->next());
      vm->execute(code);
    }
    
    vm->popOne(o);
  }
}

static void doublemap(TCollection* values1, TCollection* values2, TCollection* nvalues, Code *code, VM *vm)
{
  Value v1, v2;
  Value o;
  
  values1->iterate();
  values2->iterate();
  
  bool running = values1->hasNext() || values2->hasNext();
  
  while (running)
  {
    if (values1->hasNext() && values2->hasNext())
    {
      vm->push(values1->next());
      vm->push(values2->next());

      vm->execute(code);
      
      if (vm->popOne(o))
        nvalues->put(o);
    }
    else if (values1->hasNext())
      nvalues->put(values1->next());
    else if (values2->hasNext())
      nvalues->put(values2->next());
    
    running = values1->hasNext() || values2->hasNext();
  }
}

std::string Instruction::svalue() const
{
  if (_value.type != TYPE_OPCODE)
    return _value.svalue();
  else
  {
    switch (_value.opcode()) {
      case OP_PLUS: return "+";
      case OP_MINUS: return "-";
      case OP_TIMES: return "*";
      case OP_DIVIDE: return "/";
        
      case OP_NEG: return "_";
        
      case OP_MOD: return "//";
        
      case OP_RAND: return "@>";
      case OP_SORT: return "#>";
      case OP_SHUFFLE: return "$>";
        
      case OP_PLUS_MON: return "+.";
      case OP_PLUS_DIA: return "+:";
      case OP_MINUS_MON: return "-.";
      case OP_MINUS_DIA: return "-:";
      case OP_TIMES_MON: return "*.";
      case OP_TIMES_DIA: return "*:";
      case OP_DIVIDE_MON: return "/.";
      case OP_DIVIDE_DIA: return "/:";
        
      case OP_AND: return "&";
      case OP_NOT: return "~";
      case OP_OR: return "|";
      case OP_RSHIFT: return ">>";
      case OP_LSHIFT: return "<<";
        
      case OP_BANG: return "!";
      case OP_DBANG: return "!!";
      case OP_QUESTION: return "?";
      case OP_DQUESTION: return "??";
        
      case OP_EQUAL: return "=";
      case OP_LESSER: return "<";
      case OP_GREATER: return ">";
        
      case OP_DUPE: return "$";
      case OP_SWAP: return "%";
      case OP_DROP: return ";";
      case OP_PICK: return ",";
      case OP_RISE: return "(>";
      case OP_SINK: return "<)";
        
      case OP_PEEK: return "^";
        
      case OP_AT: return "@";
      case OP_HASH: return "#";
      case OP_AT_FRONT: return "<@";
      case OP_AT_BACK: return ">@";
      case OP_HASH_FRONT: return "<#";
      case OP_HASH_BACK: return ">#";
        
      case OP_ITER: return ".>";
      case OP_ITERI: return ".>:";
      case OP_MAP: return ".>.";
      case OP_FOLD: return ":>.";
      case OP_DMAP: return ":>:";
      case OP_CARTESIAN: return ":*.";
        
      case OP_ANY: return ".?";
      case OP_EVERY: return ":?";
        
      case OP_RECUR: return "[#]";
      case OP_RECURV: return "{#}";
      case OP_WHILE: return "<>";
    }
    
    assert(false);
  }
}

void Instruction::execute(VM *vm) const
{
  Value v1, v2, v3;

  switch (_value.opcode())
  {
    case OP_PLUS:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_STRING)
        {
          vm->push(new Value(new String(v1.string()->raw() + v2.svalue())));
        }
        else
        {        
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_CHAR, TYPE_INT): vm->push((char)(v1.character() + v2.integral())); break;
            case TYPES(TYPE_CHAR, TYPE_CHAR): vm->push(new Value(new String(std::string(1,v1.character()) + v2.character()))); break;
          }
        }
      }
      break;
    }
    case OP_MINUS:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_CHAR, TYPE_INT): vm->push((char)(v1.character() - v2.integral())); break;
        }
      }
      break;
    }
    case OP_DIVIDE:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_SET, TYPE_SET):
          {
            Set *s1 = v1.set(), *s2 = v2.set();
            const auto& d1 = s1->raw();
            const auto& d2 = s2->raw();
            Set::set_t result;
            
            for (const auto& it : d1)
              if (d2.find(it) == d2.end())
                result.insert(it);
            
            vm->push(new Value(new Set(result)));
            
            break;
          }           
        }
      }
      
      break;
    }

    case OP_OR:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_RANGE, TYPE_RANGE):
          {
            vm->push(new Range(v1.range()->raw().rangeUnion(v2.range()->raw())));
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_INT):
          {
            RangeVector rv = RangeVector(v2.integral(), v2.integral());
            vm->push(new Range(v1.range()->raw().rangeUnion(rv)));
            break;
          }
        }
      }
      break;
    }
      
    case OP_NOT:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          {
            const List::list_t& data = v1.list()->raw();
            List::list_t nv = List::list_t(data.rbegin(), data.rend());
            vm->push(new List(nv));
            break;
          }
          case TYPE_ARRAY:
          {
            const Array::array_t& ov = v1.array()->raw();
            Array::array_t nv = Array::array_t(ov.rbegin(), ov.rend());
            vm->push(new Array(nv));
            break;
          }
          case TYPE_STRING:
          {
            const std::string& ov = v1.string()->raw();
            std::string nv = std::string(ov.rbegin(), ov.rend());
            vm->push(new Value(TYPE_STRING, new String(nv)));
            break;
          }
          case TYPE_RANGE:
          {
            Array::array_t nv;
            auto iv = v1.range()->raw().concretize();
            
            nv.reserve(iv.size());
            std::transform(iv.begin(), iv.end(), std::back_inserter(nv), [] (integral_t v) { return Value(v); });

            vm->push(new Array(nv));
            break;
          }
          case TYPE_LAZY_ARRAY:
          {
            const LazyArray::data_t& array = v1.lazyArray()->raw();
            Array::array_t nv;
            nv.reserve(array.size());
            std::copy(array.begin(), array.end(), std::back_inserter(nv));

            vm->push(v1);
            vm->push(new Array(nv));
            break;
          }
          
          default: break;
        }
      }
      break;
    }
   
    case OP_QUESTION:
    {
      if (vm->popTwo(v1, v2))
      {
        {
          switch (TYPES(v1.type, v2.type))
          {
            case TYPES(TYPE_BOOL,TYPE_LAMBDA):
            {
              if (v1.boolean())
                vm->execute(v2.lambda()->code());
              break;
            }
          }

        }
      }
      break;
    }
    case OP_DQUESTION:
    {
      if (vm->popThree(v1, v2, v3))
      {
        if (v1.type == TYPE_BOOL && v2.type == TYPE_LAMBDA && v3.type == TYPE_LAMBDA)
        {
          if (v1.boolean())
            vm->execute(v2.lambda()->code());
          else
            vm->execute(v3.lambda()->code());
        }
      }
      break;
    }
      

    case OP_PEEK:
    {
      Value v = vm->peek();
      
      if (v.valid())
        std::cout << v.svalue();
      
      break;
    }
      
    case OP_RAND:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_BOOL:
          {
            vm->push((rand()%2 == 0) ? true : false);
            break;
          }
          default:
          {
            if (vm->popOne(v2))
            {
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_INT, TYPE_INT):
                {
                  integral_t m = v2.integral();
                  integral_t M = v1.integral();
                  
                  if (m > M)
                    std::swap(M, M);

                  vm->push(Util::randr(m, M));
                  break;
                }
              }
            }
            break;
          }
        }
        
      }
      break;
    }
      
      //TODO: add support for lambda based
    case OP_SORT:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_LIST:
          {
            v1.list()->raw().sort(std::less<Value>());
            //std::sort(v1.list()->begin(), v1.list()->end());
            vm->push(v1);
            break;
          }
          case TYPE_ARRAY:
          {
            std::sort(v1.array()->begin(), v1.array()->end());
            vm->push(v1);
            break;
          }
 
          default: break;
        }
      }
      break;
    }
      
    case OP_SHUFFLE:
    {
      if (vm->popOne(v1))
      {
        switch (v1.type)
        {
          case TYPE_ARRAY:
          {
            std::random_shuffle(v1.array()->begin(), v1.array()->end());
            vm->push(v1);
            break;
          }
          case TYPE_LIST:
          {
            std::vector<Value> data(v1.list()->begin(), v1.list()->end());
            std::random_shuffle(data.begin(), data.end());
            vm->push(new List(List::list_t(data.begin(), data.end())));
            break;
          }
            
          default: break;
        }
      }
      break;
    }
      
    case OP_AT:
    {
      if (vm->popOne(v2))
      {
        if (v2.type == TYPE_STACK)
        {
          List::list_t& stack = v2.stack()->raw();
          vm->push(v2);
          
          if (!stack.empty())
          {
            vm->push(stack.front());
            stack.pop_front();
          }
        }
        else if (v2.type == TYPE_QUEUE)
        {
          List::list_t& queue = v2.queue()->raw();
          vm->push(v2);
          
          if (!queue.empty())
          {
            vm->push(queue.front());
            queue.pop_front();
          }
        }
        else
        {
          if (vm->popOne(v1))
          {
            if (v1.type == TYPE_MAP)
            {
              const Map::map_t& map = v1.map()->raw();
              vm->push(v1);
              
              auto it = map.find(v2);
              
              vm->push(it != map.end() ? it->second : Value());
            }
            else
            {            
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
                {
                  LazyArray *array = v1.lazyArray();
                  integral_t i = v2.integral();
                  
                  const Value& v = array->raw().at(vm, i);
                  
                  vm->push(v1);
                  vm->push(v);
                  break;
                }
                case TYPES(TYPE_LAZY_ARRAY, TYPE_RANGE):
                {
                  const RangeVector& r = v2.range()->raw();
                  Array::utype_t nv;
                  LazyArray *ov = v1.lazyArray();
                  
                  std::vector<integral_t> iv = r.concretize();
                  nv.reserve(iv.size());
                  
                  for (size_t i = 0; i < iv.size(); ++i)
                  {
                    integral_t j = iv[i];
                    
                    if (j < 0)
                      continue;
                    
                    nv.push_back(ov->raw().at(vm, j));
                  }
                  
                  vm->push(v1);
                  vm->push(new Array(nv));            
                  break;
                }
              }
            }
          }
        }
      }
      break;
    }
      
    case OP_HASH:
    {
      if (vm->popTwo(v2, v3))
      {
        if (v2.type == TYPE_SET)
        {
          Set *set = v2.set();
          set->put(v3);
          vm->push(v2);
        }
        else if (v2.type == TYPE_STACK)
        {
          v2.stack()->raw().push_front(v3);
          vm->push(v2);
        }
        else if (v2.type == TYPE_QUEUE)
        {
          v2.queue()->raw().push_back(v3);
          vm->push(v2);
        }
        else
        {
          if (vm->popOne(v1))
          {
            if (v1.type == TYPE_MAP)
            {
              v1.map()->raw().emplace(std::make_pair(v2, v3));
              vm->push(v1);
            }
            else
            {            
              switch (TYPES(v1.type, v2.type))
              {
                case TYPES(TYPE_ARRAY, TYPE_INT):
                {
                  auto& values = v1.array()->raw();
                  integral_t i = v2.integral();
                  
                  if (i >= values.size() && i >= values.capacity())
                    values.resize(i+1, Value());
                  
                  values[v2.integral()] = v3;
                  
                  vm->push(v1);
                  break;
                }
              }
            }
          }
        }      
      }
      break;
    }
      
    case OP_AT_FRONT:
    {
      if (vm->popOne(v1))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          
          vm->push(v1);
          if (!vv.empty())
          {
            Value nv = vv.front();
            vv.pop_front();
            vm->push(nv);
          }
          else
            vm->push(Value());
        }
      }
      break;
    }
      
    case OP_AT_BACK:
    {
      if (vm->popOne(v1))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          
          vm->push(v1);
          if (!vv.empty())
          {
            Value nv = vv.back();
            vv.pop_back();
            vm->push(nv);
          }
          else
            vm->push(Value());
        }
      }
      break;
    }
      
    case OP_HASH_FRONT:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          vv.push_front(v2);
          vm->push(v1);
        }
      }
      break;
    }
      
    case OP_HASH_BACK:
    {
      if (vm->popTwo(v1, v2))
      {  
        if (v1.type == TYPE_LIST)
        {
          auto& vv = v1.list()->raw();
          vv.push_back(v2);
          vm->push(v1);
        }
        else if (v1.type == TYPE_LAMBDA)
        {
          Code* code = v1.lambda()->code();
          vm->push(new Lambda(code->append(Instruction(v2))));
        }
      }
      break;
    }
            
    case OP_ITERI:
    {
      if (vm->popTwo(v1, v2))
      {  
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Array* array = v1.array();
            Code *code = v2.lambda()->code();
            
            array->iterate();
            
            integral_t i = 0;
            
            while (array->hasNext())
            {
              auto v = array->next();
              vm->push(i++);
              vm->push(v);
              vm->execute(code);
            }
            break;
          }
          case TYPES(TYPE_MAP, TYPE_LAMBDA):
          {
            const Map::map_t& map = v1.map()->raw();
            Code *c = v2.lambda()->code();
            
            std::unordered_map<Value*, Value*>::iterator it;
            
            for (const auto& it : map)
            {
              vm->push(it.first);
              vm->push(it.second);
              vm->execute(c);
            }
          }
        }
      }
      break;
    }
            
    case OP_RECUR:
    {
      vm->push(new Lambda(vm->code()));
      break;
    }
    case OP_WHILE:
    {
      if (vm->lazy)
      {
        if (vm->popOne(v1) && v1.type == TYPE_INT)
        {
          vm->push(vm->lazy->at(vm, vm->lazy->index-v1.integral()));
        }
      }
      else if (vm->popTwo(v1, v2))
      {
        switch (TYPES(v1.type, v2.type))
        {
          case TYPES(TYPE_LAMBDA, TYPE_LAMBDA):
          {
            Code *condition = v1.lambda()->code();
            Code *body = v2.lambda()->code();
            
            vm->execute(condition);
            
            while (vm->popOne(v3) && v3.type == TYPE_BOOL && v3.boolean())
            {
              vm->execute(body);
              vm->execute(condition);
            }
            break;
          }
          case TYPES(TYPE_INT, TYPE_LAMBDA):
          {
            Code *c = v2.lambda()->code();
            int j = v1.integral();
            
            for (int i = 0; i < j; ++i)
            {
              vm->execute(c);
            }
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            const auto& values = v1.list()->raw();
            Code *c = v2.lambda()->code();
            std::list<Value*>::iterator it;
            
            List::utype_t ot, of;
            
            for (const auto& v : values)
            {
              vm->push(v);
              vm->execute(c);
              
              if (vm->popOne(v3) && v3.type.traits().to_bool(v3))
                ot.push_back(v);
              else
                of.push_back(v);
            }
            
            vm->push(new List(of));
            vm->push(new List(ot));
            break;
            
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            const auto& values = v1.array()->raw();
            Code *c = v2.lambda()->code();
            std::list<Value*>::iterator it;
            
            Array::utype_t ot, of;
            
            for (const auto& v : values)
            {
              vm->push(v);
              vm->execute(c);
              
              if (vm->popOne(v3) && v3.type.traits().to_bool(v3))
                ot.push_back(v);
              else
                of.push_back(v);
            }
            
            vm->push(new Array(of));
            vm->push(new Array(ot));
            break;
            
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_LAMBDA):
          {
            u32 s = 0;
            LazyArray *lazy = v1.lazyArray();
            Lambda *lambda = v2.lambda();
            bool finished = false;
            
            do {
              const Value& v = lazy->raw().at(vm, s);
              vm->push(v);
              vm->execute(lambda->code());
              
              if (vm->popOne(v3))
              {
                if (v3.type.traits().to_bool(v3))
                  finished = true;
              }
              
              ++s;
              
            } while (!finished);
            
            lazy->raw().shrinkBy(1);
            vm->push(v1);
            
            break;
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
          {
            LazyArray *lazy = v1.lazyArray();
            integral_t i = v2.integral();
            
            for (integral_t j = lazy->raw().size(); j < i; ++j)
              lazy->raw().at(vm, j);

            vm->push(v1);
            
            break;
          }
        }
      }
      break;
    }
      
    case OP_DBANG:
    {
      if (vm->popTwo(v1, v2))
      {
        if (v1.type == TYPE_LAMBDA)
        {
          /* curryfication converts a function x,y -> z into x -> (y -> z) */
          Code* code = v1.lambda()->code();
          
          break;
        }
      }
      break;
    }
      
    case OP_RAISE_STACK: { vm->raiseStack(); break; }
    case OP_LOWER_STACK: { vm->lowerStack(); break; }
      
    case OP_RAISE_STACKV:
    { 
      if (vm->popOne(v1) && v1.type == TYPE_INT)
      {
        integral_t t = v1.integral();
        
        if (vm->stackSize() >= t)
        {
          std::list<Value> tmpStack;
          
          for (int i = 0; i < t; ++i)
            tmpStack.push_back(vm->pop());
          
          vm->raiseStack();
          
          for (int i = 0; i < t; ++i)
          {
            vm->push(tmpStack.back());
            tmpStack.pop_back();
          }
        }
      }
      break;
    }
      
    case OP_LOWER_STACKV:
    { 
      if (vm->popOne(v1) && v1.type == TYPE_INT)
      {
        integral_t t = v1.integral();
        
        if (vm->stackSize() >= t)
        {
          std::list<Value> tmpStack;
          
          for (int i = 0; i < t; ++i)
            tmpStack.push_back(vm->pop());
          
          vm->lowerStack();
          
          for (int i = 0; i < t; ++i)
          {
            vm->push(tmpStack.back());
            tmpStack.pop_back();
          }
        }
      }
      break;
    }
      
    case OP_STRING_CAST:
    {
      if (vm->popOne(v1))
      {
        vm->push(new String(v1.svalue()));
      }
    }
  }
}
