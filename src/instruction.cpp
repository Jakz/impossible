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

std::string Instruction::svalue() const
{
  return _value.svalue();
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
            vm->push(new String(nv));
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
      if (vm->popThree(v1, v2, v3))
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
            
            //TODO: push tuple<index, value>
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
      if (vm->record().lazy)
      {
        if (vm->popOne(v1) && v1.type == TYPE_INT)
        {
          auto&& index = vm->record().lazy->index - v1.integral();
          auto&& value = vm->record().lazy->at(vm, index);
          vm->push(value);
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
