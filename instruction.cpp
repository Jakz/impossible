/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 12/22/12
 */

#include "instruction.h"

#include "vm.h"

static void iter(TCollection *values, Code *code, VM *vm)
{
  values->iterate();
  
  Value *v;
  
  while ((v = values->next()))
  {
    vm->push(v);
    vm->execute(code);
  }
}

static void map(TCollection* values, TCollection* nvalues, Code *code, VM *vm)
{
  values->iterate();
  
  Value *v;
  Value *o;
  
  while ((v = values->next()))
  {
    vm->push(v);
    vm->execute(code);
    
    if (vm->popOne(&o))
      nvalues->put(o);
  }
}

static void fold(TCollection* values, Value **o, Code *code, VM *vm)
{
  vm->push(*o);
  
  Value *v;
  values->iterate();
  while ((v = values->next()))
  {
    vm->push(v);
    vm->execute(code);
  }
  
  vm->popOne(o);
}

static void sfold(TCollection* values, Value **o, Code *code, VM *vm)
{
  values->iterate();
  
  Value *v = values->next();

  if (v)
  {
    vm->push(v);
    while ((v = values->next()))
    {
      vm->push(v);
      vm->execute(code);
    }
    
    vm->popOne(o);
  }
}

static void doublemap(TCollection* values1, TCollection* values2, TCollection* nvalues, Code *code, VM *vm)
{
  Value *v1, *v2;
  Value *o;
  
  values1->iterate();
  values2->iterate();
  
  bool running = values1->hasNext() || values2->hasNext();
  
  while (running)
  {
    v1 = values1->next();
    v2 = values2->next();
    
    if (v1 && v2)
    {
      vm->push(v1);
      vm->push(v2);
      vm->execute(code);
      
      if (vm->popOne(&o))
        nvalues->put(o);
    }
    else if (v1)
      nvalues->put(v1);
    else
      nvalues->put(v2);
    
    running = values1->hasNext() || values2->hasNext();
  }
}

static void cartesian(TCollection *values1, TCollection* values2, TCollection* nvalues, Code *code, VM *vm)
{
  Value *v1, *v2;
  Value *o;
  
  values1->iterate();
  values2->iterate();

  while ((v1 = values1->next()))
  {
    while ((v2 = values2->next()))
    {
      vm->push(v1);
      vm->push(v2);
      vm->execute(code);
      
      if (vm->popOne(&o))
        nvalues->put(o);
    }
    
    values2->iterate();
  }
}

void PushInstruction::execute(VM *vm) const
{
  vm->push(value);
}

string PushInstruction::svalue() const { return value->svalue().c_str(); }


string OpcodeInstruction::svalue() const
{
  switch (opcode) {
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
    case OP_LSHIFT: return ">>";
      
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
}


void OpcodeInstruction::execute(VM *vm) const
{
  Value *v1, *v2, *v3;
  
  switch (opcode)
  {
    case OP_DUPE:
    {
      v1 = vm->peek();
      
      if (v1)
      {
        vm->push(v1->clone());
      }
      
      break;
    }
    case OP_SWAP:
    {
      if (vm->popTwo(&v1, &v2))
      {
        vm->push(v2);
        vm->push(v1);
      }
      break;
    }
    case OP_DROP:
    {
      if (vm->popOne(&v1))
      {
        
      }
      break;
    }
    case OP_PICK:
    {
      if (vm->popOne(&v1) && v1->type == TYPE_INT)
      {
        int i = ((Int*)v1)->get();
        vm->push(vm->pick(i));
      }
      break;
    }
    case OP_RISE:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        vm->push(v2);
        vm->push(v3);
        vm->push(v1);
      }
      break;
    }
    case OP_SINK:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        vm->push(v3);
        vm->push(v1);
        vm->push(v2);
      }
      break;
    }
    
    
    case OP_PLUS:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        if (v1->type == TYPE_STRING)
        {
          vm->push(new String(((String*)v1)->get() += v2->svalue()));
        }
        else
        {        
          switch (TYPES(v1->type, v2->type))
          {
            case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() + ((Int*)v2)->get())); break;
            case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Float(((Float*)v1)->get() + ((Int*)v2)->get())); break;
            case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Float(((Int*)v1)->get() + ((Float*)v2)->get())); break;
            case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Float(((Float*)v1)->get() + ((Float*)v2)->get())); break;
            case TYPES(TYPE_CHAR, TYPE_INT): vm->push(new Char(((Char*)v1)->get() + ((Int*)v2)->get())); break;
            case TYPES(TYPE_CHAR, TYPE_CHAR): vm->push(new String(string(1,((Char*)v1)->get()) += ((Char*)v2)->get())); break;
          }
        }
      }
      break;
    }
    case OP_MINUS:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() - ((Int*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Float(((Float*)v1)->get() - ((Int*)v2)->get())); break;
          case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Float(((Int*)v1)->get() - ((Float*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Float(((Float*)v1)->get() - ((Float*)v2)->get())); break;
          case TYPES(TYPE_CHAR, TYPE_INT): vm->push(new Char(((Char*)v1)->get() - ((Int*)v2)->get())); break;
        }
      }
      break;
    }
    case OP_TIMES:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() * ((Int*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Float(((Float*)v1)->get() * ((Int*)v2)->get())); break;
          case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Float(((Int*)v1)->get() * ((Float*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Float(((Float*)v1)->get() * ((Float*)v2)->get())); break;
        }
      }
      break;
    }
    case OP_DIVIDE:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() / ((Int*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Float(((Float*)v1)->get() / ((Int*)v2)->get())); break;
          case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Float(((Int*)v1)->get() / ((Float*)v2)->get())); break;
          case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Float(((Float*)v1)->get() / ((Float*)v2)->get())); break;            
          case TYPES(TYPE_SET, TYPE_SET):
          {
            unordered_set<Value*>* s1 = ((Set*)v1)->get();
            unordered_set<Value*>* s2 = ((Set*)v2)->get();
            unordered_set<Value*>* sr = new unordered_set<Value*>();
            
            unordered_set<Value*>::iterator it;
            
            for (it = s1->begin(); it != s1->end(); ++it)
            {
              if (s2->find(*it) == s2->end())
                sr->insert(*it);
            }
            
            vm->push(new Set(sr));
            break;
          }           
        }
      }
      
      break;
    }
    case OP_NEG:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)  
        {
          case TYPE_INT: vm->push(new Int(-((Int*)v1)->get())); break;
          case TYPE_FLOAT: vm->push(new Float(-((Float*)v1)->get())); break;
          case TYPE_LIST: vm->push(v1); vm->push(new Int(((List*)v1)->size())); break;
          case TYPE_ARRAY: vm->push(v1); vm->push(new Int(((Array*)v1)->size())); break;
          case TYPE_LAZY_ARRAY: vm->push(v1); vm->push(new Int(((LazyArray*)v1)->size())); break;
          case TYPE_STRING: vm->push(v1); vm->push(new Int(((String*)v1)->size())); break;
          case TYPE_STACK: vm->push(v1); vm->push(new Int(((Stack*)v1)->size())); break;
          case TYPE_QUEUE: vm->push(v1); vm->push(new Int(((Queue*)v1)->size())); break;
          case TYPE_MAP: vm->push(v1); vm->push(new Int(((Map*)v1)->size())); break;
          default: break;
        }
      }
      break;
    }
      
      
    case OP_MOD:
    {
      if (vm->popOne(&v2))
      {
        switch (v2->type)
        {
          case TYPE_FLOAT:
          {
            double f, i;
            f = modf(((Float*)v2)->get(), &i);
            vm->push(new Float(i));
            vm->push(new Float(f));
            break;
          }
          default:
          {
            
            if (vm->popOne(&v1))
            {
              switch (TYPES(v1->type, v2->type))
              {
                case TYPES(TYPE_INT, TYPE_INT):
                {
                  vm->push(new Int(((Int*)v1)->get() % ((Int*)v2)->get()));
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
      
    case OP_AND:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_BOOL, TYPE_BOOL): vm->push(new Bool(((Bool*)v1)->get() & ((Bool*)v2)->get())); break;
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() & ((Int*)v2)->get())); break;
          case TYPES(TYPE_SET, TYPE_SET):
          {
            unordered_set<Value*>* s1 = ((Set*)v1)->get();
            unordered_set<Value*>* s2 = ((Set*)v2)->get();
            unordered_set<Value*>* sr = new unordered_set<Value*>();
            
            unordered_set<Value*>::iterator it;
            
            for (it = s1->begin(); it != s1->end(); ++it)
            {
              if (s2->find(*it) != s2->end())
                sr->insert(*it);
            }
            
            vm->push(new Set(sr));
            break;
          }
        }
      }
      break;
    }
      
    case OP_OR:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_BOOL, TYPE_BOOL): vm->push(new Bool(((Bool*)v1)->get() | ((Bool*)v2)->get())); break;
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() | ((Int*)v2)->get())); break;
          case TYPES(TYPE_RANGE, TYPE_RANGE):
          {
            vm->push(new Range(((Range*)v1)->get().rangeUnion(((Range*)v2)->get())));
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_INT):
          {
            RangeVector rv = RangeVector(((Int*)v2)->get(), ((Int*)v2)->get());
            vm->push(new Range(((Range*)v1)->get().rangeUnion(rv)));
            break;
          }
            
          case TYPES(TYPE_SET, TYPE_SET):
          {
            unordered_set<Value*>* s1 = ((Set*)v1)->get();
            unordered_set<Value*>* s2 = ((Set*)v2)->get();
            unordered_set<Value*>* sr = new unordered_set<Value*>();
            
            unordered_set<Value*>::iterator it;
            
            for (it = s1->begin(); it != s1->end(); ++it)
              sr->insert(*it);
            
            for (it = s2->begin(); it != s2->end(); ++it)
              sr->insert(*it);
            
            vm->push(new Set(sr));
            break;
          }
        }
      }
      break;
    }
      
    case OP_NOT:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_BOOL: vm->push(new Bool(!((Bool*)v1)->get())); break;
          case TYPE_INT: vm->push(new Int(~((Int*)v1)->get())); break;
          case TYPE_FLOAT: vm->push(new Float(1.0 / ((Float*)v1)->get())); break;
          case TYPE_LIST:
          {
            list<Value*>* ov = ((List*)v1)->get();
            list<Value*>* nv = new list<Value*>(ov->rbegin(),ov->rend());
            vm->push(new List(nv));
            break;
          }
          case TYPE_ARRAY:
          {
            vector<Value*>* ov = ((Array*)v1)->get();
            vector<Value*>* nv = new vector<Value*>(ov->rbegin(),ov->rend());
            vm->push(new Array(nv));
            break;
          }
          case TYPE_STRING:
          {
            string ov = ((String*)v1)->get();
            string nv = string(ov.rbegin(), ov.rend());
            vm->push(new String(nv));
            break;
          }
          case TYPE_RANGE:
          {
            Range *r = (Range*)v1;       
            vector<Value*>* nv = new vector<Value*>();
            vector<int> iv = r->get().concretize();
            nv->reserve(iv.size());
            
            for (int i = 0; i < iv.size(); ++i)
              nv->push_back(new Int(iv.at(i)));
            
            vm->push(new Array(nv));
            break;
          }
          case TYPE_LAZY_ARRAY:
          {
            LazyArray *array = (LazyArray*)v1;
            vector<Value*>* nv = new vector<Value*>();
            nv->reserve(array->size());
            
            for (int i = 0; i < array->size(); ++i)
              nv->push_back(array->get().at(vm, i));
            
            vm->push(v1);
            vm->push(new Array(nv));
            break;
          }
          
          default: break;
        }
      }
      break;
    }
    case OP_RSHIFT:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() >> ((Int*)v2)->get())); break;
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            list<Value*>* v = ((List*)v1)->get();
            Code *c = ((Lambda*)v2)->get();
            list<Value*>::iterator it;
            
            list<Value*>* ot = new list<Value*>();
            for (it = v->begin(); it != v->end(); ++it)
            {
              vm->push(*it);
              vm->execute(c);
              
              if (vm->popOne(&v3))
              {
                if (v3->type == TYPE_BOOL && ((Bool*)v3)->get())
                  ot->push_back(*it);
              }
            }
            vm->push(new List(ot));
            break;
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            vector<Value*>* v = ((Array*)v1)->get();
            Code *c = ((Lambda*)v2)->get();
            vector<Value*>::iterator it;
            
            vector<Value*>* ot = new vector<Value*>();
            for (it = v->begin(); it != v->end(); ++it)
            {
              vm->push(*it);
              vm->execute(c);
              
              if (vm->popOne(&v3))
              {
                if (v3->type == TYPE_BOOL && ((Bool*)v3)->get())
                  ot->push_back(*it);
              }
            }
            vm->push(new Array(ot));
            break;
            break;
          }
        }
      }
      break;
    }
    case OP_LSHIFT:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        if (v1->type == TYPE_ARRAY)
        {
          vector<Value*>* vs = ((Array*)v1)->get();
          vs->assign(vs->size(), v2->clone());
          vm->push(v1);
          
        }
        else
        {
          switch (TYPES(v1->type, v2->type))
          {
            case TYPES(TYPE_INT, TYPE_INT): vm->push(new Int(((Int*)v1)->get() << ((Int*)v2)->get())); break;
          }
        }
      }
      break;
    }
      
      
    case OP_EQUAL:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        vm->push(new Bool(v1->equals(v2)));
      }
      break;
    }
    case OP_LESSER:
    {
      if (vm->popOne(&v2))
      {
        if (v2->type == TYPE_LIST)
        {
          list<Value*>* v = ((List*)v2)->get();
          vm->push(v2);
          
          if (!v->empty())
          {
            list<Value*>::iterator it = min_element(v->begin(), v->end(), less<Value*>());
            vm->push(*it);
          }
        }
        else if (v2->type == TYPE_ARRAY)
        {
          vector<Value*>* v = ((Array*)v2)->get();
          vm->push(v2);
          
          if (!v->empty())
          {
            vector<Value*>::iterator it = min_element(v->begin(), v->end(), less<Value*>());
            vm->push(*it);
          }
        }
        else
        {
          if (vm->popOne(&v1))
          {  
            switch (TYPES(v1->type, v2->type))
            {
              case TYPES(TYPE_INT, TYPE_INT): vm->push(new Bool(((Int*)v1)->get() < ((Int*)v2)->get())); break;
              case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Bool(((Float*)v1)->get() < ((Int*)v2)->get())); break;
              case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Bool(((Int*)v1)->get() < ((Float*)v2)->get())); break;
              case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Bool(((Float*)v1)->get() < ((Float*)v2)->get())); break;
            }
          }
        }
      }
      break;
    }
    case OP_GREATER:
    {
      if (vm->popOne(&v2))
      {
        if (v2->type == TYPE_LIST)
        {
          list<Value*>* v = ((List*)v2)->get();          
          vm->push(v2);
          
          if (!v->empty())
          {
            list<Value*>::iterator it = max_element(v->begin(), v->end(), less<Value*>());
            vm->push(*it);
          }
        }
        else if (v2->type == TYPE_ARRAY)
        {
          vector<Value*>* v = ((Array*)v2)->get();
          vm->push(v2);
          
          if (!v->empty())
          {
            vector<Value*>::iterator it = max_element(v->begin(), v->end(), less<Value*>());
            vm->push(*it);
          }
        }
        else
        {
          if (vm->popOne(&v1))
          {   
            switch (TYPES(v1->type, v2->type))
            {
              case TYPES(TYPE_INT, TYPE_INT): vm->push(new Bool(((Int*)v1)->get() > ((Int*)v2)->get())); break;
              case TYPES(TYPE_FLOAT, TYPE_INT): vm->push(new Bool(((Float*)v1)->get() > ((Int*)v2)->get())); break;
              case TYPES(TYPE_INT, TYPE_FLOAT): vm->push(new Bool(((Int*)v1)->get() > ((Float*)v2)->get())); break;
              case TYPES(TYPE_FLOAT, TYPE_FLOAT): vm->push(new Bool(((Float*)v1)->get() > ((Float*)v2)->get())); break;
            }
          }
        }
      }
      break;
    }      
      
      
      
    case OP_BANG:
    {
      if (vm->popOne(&v1))
      {
        if (v1->type == TYPE_LAMBDA)
          vm->execute(((Lambda*)v1)->get());
        else if (v1->type == TYPE_INT)
        {
          double res = 1.0;
          double v = ((Int*)v1)->get();
          
          while (v > 1)
            res *= v--;
          
          vm->push(new Float(res));
        }
      }
      break;
    }
    case OP_QUESTION:
    {
      if (vm->popTwo(&v1, &v2))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            list<Value*>* vv = ((List*)v1)->get();
            vm->push(v1);
            if (find(vv->begin(), vv->end(), v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
          case TYPE_ARRAY:
          {
            vector<Value*>* vv = ((Array*)v1)->get();
            vm->push(v1);
            if (find(vv->begin(), vv->end(), v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
          case TYPE_STACK:
          {
            list<Value*>* vv = ((Stack*)v1)->get();
            vm->push(v1);
            if (find(vv->begin(), vv->end(), v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
          case TYPE_QUEUE:
          {
            list<Value*>* vv = ((Queue*)v1)->get();
            vm->push(v1);
            if (find(vv->begin(), vv->end(), v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
          case TYPE_SET:
          {
            unordered_set<Value*>* vv = ((Set*)v1)->get();
            vm->push(v1);
            if (vv->find(v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
          case TYPE_MAP:
          {
            unordered_map<Value*,Value*>* vv = ((Map*)v1)->get();
            vm->push(v1);
            if (vv->find(v2) != vv->end())
              vm->push(new Bool(true));
            else
              vm->push(new Bool(false));
            break;
          }
            
          default:
          {
            switch (TYPES(v1->type, v2->type))
            {
              case TYPES(TYPE_BOOL,TYPE_LAMBDA):
              {
                if (((Bool*)v1)->get())
                  vm->execute(((Lambda*)v2)->get());
                break;
              }
            }
            break;
          }
        }
      }
      break;
    }
    case OP_DQUESTION:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        if (v1->type == TYPE_BOOL && v2->type == TYPE_LAMBDA && v3->type == TYPE_LAMBDA)
        {
          if (((Bool*)v1)->get())
            vm->execute(((Lambda*)v2)->get());
          else
            vm->execute(((Lambda*)v3)->get());
        }
      }
      break;
    }
      

    case OP_PEEK:
    {
      Value *v = vm->peek();
      
      if (v)
        cout << v->svalue();
      
      break;
    }
      
    case OP_RAND:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_BOOL:
          {
            vm->push(new Bool(rand()%2 == 0 ? true : false));
            break;
          }
          case TYPE_RANGE:
          {
            vector<int> iv = ((Range*)v1)->get().concretize();
            int c = Util::randi(0, (int)iv.size());
            vm->push(new Int(iv.at(c)));
            break;
          }
          case TYPE_ARRAY:
          {
            Array *vv = (Array*)v1;
            int size = vv->size();
            vector<Value*>::iterator it = vv->get()->begin();
            std::advance(it, Util::randi(0, size));
            vm->push(*it);
            break;
          }
          case TYPE_LIST:
          {
            List *vv = (List*)v1;
            int size = vv->size();
            list<Value*>::iterator it = vv->get()->begin();
            std::advance(it, Util::randi(0, size));
            vm->push(*it);
            break;
          }
          case TYPE_SET:
          {
            Set *vv = (Set*)v1;
            int size = vv->size();
            unordered_set<Value*>::iterator it = vv->get()->begin();
            std::advance(it, Util::randi(0, size));
            vm->push(*it);
            break;
          }
          default:
          {
            if (vm->popOne(&v2))
            {
              switch (TYPES(v1->type, v2->type))
              {
                case TYPES(TYPE_INT, TYPE_INT):
                {
                  int m = ((Int*)v2)->get();
                  int M = ((Int*)v1)->get();
                  
                  if (m > M)
                  {
                    m ^= M;
                    M ^= m;
                    m ^= M;
                  }
                  
                  vm->push(new Int(Util::randi(m, M)));
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
      
    case OP_SORT:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            List *v = (List*)v1;
            v->get()->sort(less<Value*>());
            vm->push(v1);
            break;
          }
          case TYPE_ARRAY:
          {
            Array *v = (Array*)v1;
            sort(v->get()->begin(), v->get()->end(), less<Value*>());
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
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_ARRAY:
          {
            Array *v = (Array*)v1;
            random_shuffle(v->get()->begin(), v->get()->end());
            vm->push(v1);
            break;
          }
          case TYPE_LIST:
          {
            List *v = (List*)v1;
            vector<Value*> data(v->get()->begin(), v->get()->end());
            random_shuffle(data.begin(), data.end());
            List *nv = new List(new list<Value*>(data.begin(), data.end()));
            vm->push(nv);
            break;
          }
            
          default: break;
        }
      }
      break;
    }
      
    case OP_AT:
    {
      if (vm->popOne(&v2))
      {
        if (v2->type == TYPE_STACK)
        {
          Stack *stack = (Stack*)v2;
          vm->push(stack);
          
          if (!stack->empty())
          {
            vm->push(stack->get()->front());
            stack->get()->pop_front();
          }
        }
        else if (v2->type == TYPE_QUEUE)
        {
          Queue *stack = (Queue*)v2;
          vm->push(stack);
          
          if (!stack->empty())
          {
            vm->push(stack->get()->front());
            stack->get()->pop_front();
          }
        }
        else
        {
          if (vm->popOne(&v1))
          {
            if (v1->type == TYPE_MAP)
            {
              Map* map = (Map*)v1;
              unordered_map<Value*, Value*>::iterator it = map->get()->find(v2);
              
              vm->push(v1);
              
              if (it != map->get()->end())
                vm->push(it->second);
              else
                vm->push(TValue<void*>::NIL->clone());
            }
            else
            {            
              switch (TYPES(v1->type, v2->type))
              {
                case TYPES(TYPE_ARRAY, TYPE_INT):
                {
                  Array *values = (Array*)v1;
                  int i = ((Int*)v2)->get();
                  
                  vm->push(v1);
                  
                  if (i < values->size())
                    vm->push(values->get()->at(i));
                  else
                    vm->push(TValue<void*>::NIL->clone());
                  
                  
                  break;
                }
                case TYPES(TYPE_ARRAY, TYPE_RANGE):
                {
                  RangeVector r = ((Range*)v2)->get();
                  vector<Value*>* nv = new vector<Value*>();
                  vector<Value*>* ov = ((Array*)v1)->get();
                  
                  vector<int> iv = r.concretize();
                  nv->reserve(iv.size());
                  
                  for (int i = 0; i < iv.size(); ++i)
                  {
                    int j = iv[i];
                    
                    if (j < 0)
                      j += (u32)ov->size();
                    
                    if (j < ov->size())
                      nv->push_back(ov->at(j));
                  }
                  
                  vm->push(v1);
                  vm->push(new Array(nv));            
                  break;
                }
                case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
                {
                  LazyArray *array = (LazyArray*)v1;
                  int i = ((Int*)v2)->get();
                  
                  Value *v = array->get().at(vm, i);
                  
                  vm->push(v1);
                  vm->push(v);
                  break;
                }
                case TYPES(TYPE_LAZY_ARRAY, TYPE_RANGE):
                {
                  RangeVector r = ((Range*)v2)->get();
                  vector<Value*>* nv = new vector<Value*>();
                  LazyArray *ov = (LazyArray*)v1;
                  
                  vector<int> iv = r.concretize();
                  nv->reserve(iv.size());
                  
                  for (int i = 0; i < iv.size(); ++i)
                  {
                    int j = iv[i];
                    
                    if (j < 0)
                      continue;
                    
                    nv->push_back(ov->get().at(vm, j));
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
      if (vm->popTwo(&v2, &v3))
      {
        if (v2->type == TYPE_SET)
        {
          Set *set = (Set*)v2;
          set->put(v3);
          vm->push(v2);
        }
        else if (v2->type == TYPE_STACK)
        {
          Stack *stack = (Stack*)v2;
          stack->get()->push_front(v3);
          vm->push(v2);
        }
        else if (v2->type == TYPE_QUEUE)
        {
          Queue *queue = (Queue*)v2;
          queue->get()->push_back(v3);
          vm->push(v2);
        }
        else
        {
          if (vm->popOne(&v1))
          {
            if (v1->type == TYPE_MAP)
            {
              Map *map = (Map*)v1;
              map->get()->insert(pair<Value*,Value*>(v2,v3));
              vm->push(v1);
            }
            else
            {            
              switch (TYPES(v1->type, v2->type))
              {
                case TYPES(TYPE_ARRAY, TYPE_INT):
                {
                  Array *values = (Array*)v1;
                  int i = ((Int*)v2)->get();
                  
                  if (i >= values->size() && i >= values->get()->capacity())
                    values->get()->resize(i+1, TValue<void*>::NIL->clone());
                  
                  (*values->get())[((Int*)v2)->get()] = v3;
                  
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
      if (vm->popOne(&v1))
      {  
        if (v1->type == TYPE_LIST)
        {
          list<Value*>* vv = ((List*)v1)->get();
          
          vm->push(v1);
          if (!vv->empty())
          {
            Value *nv = vv->front();
            vv->pop_front();
            vm->push(nv);
          }
          else
            vm->push(TValue<void*>::NIL->clone());
        }
      }
      break;
    }
      
    case OP_AT_BACK:
    {
      if (vm->popOne(&v1))
      {  
        if (v1->type == TYPE_LIST)
        {
          list<Value*>* vv = ((List*)v1)->get();
          
          vm->push(v1);
          if (!vv->empty())
          {
            Value *nv = vv->back();
            vv->pop_back();
            vm->push(nv);
          }
          else
            vm->push(TValue<void*>::NIL->clone());
        }
      }
      break;
    }
      
    case OP_HASH_FRONT:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        if (v1->type == TYPE_LIST)
        {
          list<Value*>* vv = ((List*)v1)->get();          
          vv->push_front(v2);          
          vm->push(v1);
        }
      }
      break;
    }
      
    case OP_HASH_BACK:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        if (v1->type == TYPE_LIST)
        {
          list<Value*>* vv = ((List*)v1)->get();          
          vv->push_back(v2);          
          vm->push(v1);
        }
      }
      break;
    }
      
    case OP_ITER:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            iter((List*)v1,((Lambda*)v2)->get(),vm);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            iter((Array*)v1,((Lambda*)v2)->get(),vm);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            iter((Set*)v1, ((Lambda*)v2)->get(), vm);
            break;
          }
          case TYPES(TYPE_STRING, TYPE_LAMBDA):
          {
            iter((String*)v1, ((Lambda*)v2)->get(), vm);
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_LAMBDA):
          {
            iter((Range*)v1, ((Lambda*)v2)->get(), vm);
            break;
          }
          case TYPES(TYPE_MAP, TYPE_LAMBDA):
          {
            iter((Map*)v1, ((Lambda*)v2)->get(), vm);
            break;
          }
        }
      }
      break;
    }
      
    case OP_ITERI:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Array *values = (Array*)v1;
            Code *code = ((Lambda*)v2)->get();
            
            values->iterate();
            
            Value *v;
            int i = 0;
            
            while ((v = values->next()))
            {
              vm->push(new Int(i++));
              vm->push(v);
              vm->execute(code);
            }
            break;
          }
          case TYPES(TYPE_MAP, TYPE_LAMBDA):
          {
            unordered_map<Value*, Value*>* v = ((Map*)v1)->get();
            Code *c = ((Lambda*)v2)->get();
            
            unordered_map<Value*, Value*>::iterator it;
            
            for (it = v->begin(); it != v->end(); ++it)
            {
              vm->push(it->first);
              vm->push(it->second);
              vm->execute(c);
            }
          }
        }
      }
      break;
    }
      
    case OP_MAP:
    {
      if (vm->popTwo(&v1, &v2))
      {  
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            Lambda *lambda = (Lambda*)v2;
            List *values = (List*)v1;
            List *nvalues = new List();

            map(values,nvalues,lambda->get(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Lambda *lambda = (Lambda*)v2;
            Array* values = (Array*)v1;
            Array* nvalues = new Array();
            
            map(values,nvalues,lambda->get(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_RANGE, TYPE_LAMBDA):
          {
            Lambda *lambda = (Lambda*)v2;
            Range* values = (Range*)v1;
            Array* nvalues = new Array();
            
            map(values,nvalues,lambda->get(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Lambda *lambda = (Lambda*)v2;
            Set* values = (Set*)v1;
            Set* nvalues = new Set();
            
            map(values,nvalues,lambda->get(),vm);
            
            vm->push(nvalues);
            break;
          }
          case TYPES(TYPE_STRING, TYPE_LAMBDA):
          {
            Lambda *lambda = (Lambda*)v2;
            String* values = (String*)v1;
            String* nvalues = new String();
            
            map(values,nvalues,lambda->get(),vm);
            
            vm->push(nvalues);
            break;
          }
        }
      }
      
      break;
    }

    case OP_FOLD:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            fold((List*)v1, &v3, ((Lambda*)v2)->get(), vm);
            vm->push(v3);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            fold((Array*)v1, &v3, ((Lambda*)v2)->get(), vm);
            vm->push(v3);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            fold((Set*)v1, &v3, ((Lambda*)v2)->get(), vm);
            vm->push(v3);
            break;
          }
        }
      }
      break;
    }
            
    case OP_DMAP:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        if (v3->type == TYPE_LAMBDA)
        {
          switch (TYPES(v1->type, v2->type))
          {
            case TYPES(TYPE_LIST, TYPE_LIST):
            {
              List *list = new List();
              doublemap((List*)v1, (List*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_LIST, TYPE_ARRAY):
            {
              List *list = new List();
              doublemap((List*)v1, (Array*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_ARRAY, TYPE_LIST):
            {
              Array *list = new Array();
              doublemap((Array*)v1, (List*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_ARRAY, TYPE_ARRAY):
            {
              Array *list = new Array();
              doublemap((Array*)v1, (Array*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
          }
        }
        
      }
      break;
    }
      
    case OP_CARTESIAN:
    {
      if (vm->popThree(&v1, &v2, &v3))
      {
        if (v3->type == TYPE_LAMBDA)
        {
          switch (TYPES(v1->type, v2->type))
          {
            case TYPES(TYPE_ARRAY, TYPE_ARRAY):
            {
              Array *list = new Array();
              cartesian((Array*)v1, (Array*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
            case TYPES(TYPE_RANGE, TYPE_RANGE):
            {
              Array *list = new Array();
              cartesian((Range*)v1, (Range*)v2, list, ((Lambda*)v3)->get(), vm);
              vm->push(list);
              break;
            }
          }
        }
      }
        
      break;
    }
      
    case OP_ANY:
    {
      if (vm->popTwo(&v1, &v2))
      {
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT):
          case TYPES(TYPE_FLOAT, TYPE_FLOAT):
          case TYPES(TYPE_INT, TYPE_FLOAT):
          case TYPES(TYPE_FLOAT, TYPE_INT):
          {
            double x = v1->type == TYPE_INT ? ((Int*)v1)->get() : ((Float*)v1)->get();
            double y = v2->type == TYPE_INT ? ((Int*)v2)->get() : ((Float*)v2)->get();            
            vm->push(x < y ? v1 : v2);
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            Value *v = new Bool(false);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_OR));
            fold((List*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Value *v = new Bool(false);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_OR));
            fold((Array*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Value *v = new Bool(false);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_OR));
            fold((Set*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
        }
      }
      break;
    }
      
    case OP_EVERY:
    {
      if (vm->popTwo(&v1, &v2))
      {
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_INT, TYPE_INT):
          case TYPES(TYPE_FLOAT, TYPE_FLOAT):
          case TYPES(TYPE_INT, TYPE_FLOAT):
          case TYPES(TYPE_FLOAT, TYPE_INT):
          {
            double x = v1->type == TYPE_INT ? ((Int*)v1)->get() : ((Float*)v1)->get();
            double y = v2->type == TYPE_INT ? ((Int*)v2)->get() : ((Float*)v2)->get();            
            vm->push(x > y ? v1 : v2);
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            Value *v = new Bool(true);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_AND));
            fold((List*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            Value *v = new Bool(true);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_AND));
            fold((Array*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
          case TYPES(TYPE_SET, TYPE_LAMBDA):
          {
            Value *v = new Bool(true);
            Code *nc = ((Lambda*)v2)->get()->append(new OpcodeInstruction(OP_AND));
            fold((Set*)v1, &v, nc, vm);
            vm->push(v);
            break;
          }
        }
      }
      break;
    }
      
    case OP_PLUS_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_PLUS))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_PLUS_MON:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            Value *o = NULL;
            sfold((List*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_PLUS)), vm);
            if (o)
              vm->push(o);
            break;
          }
          case TYPE_ARRAY:
          {
            Value *o = NULL;
            sfold((Array*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_PLUS)), vm);
            if (o)
              vm->push(o);
            break;
          }
          default: break;
        }
      }
      
      break;
    }
      
    case OP_MINUS_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_MINUS))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_MINUS_MON:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            Value *o = NULL;
            sfold((List*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_MINUS)), vm);
            if (o)
              vm->push(o);
            break;
          }
          case TYPE_ARRAY:
          {
            Value *o = NULL;
            sfold((Array*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_MINUS)), vm);
            if (o)
              vm->push(o);
            break;
          }
          default: break;
        }
      }
      
      break;
    }
      
    case OP_TIMES_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_TIMES))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_TIMES_MON:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            Value *o = NULL;
            sfold((List*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_TIMES)), vm);
            if (o)
              vm->push(o);
            break;
          }
          case TYPE_ARRAY:
          {
            Value *o = NULL;
            sfold((Array*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_TIMES)), vm);
            if (o)
              vm->push(o);
            break;
          }
          default: break;
        }
      }
      
      break;
    }
      
    case OP_DIVIDE_DIA:
    {
      vm->push(new Lambda(new CodeStandard(new OpcodeInstruction(OP_DIVIDE))));
      OpcodeInstruction(OP_DMAP).execute(vm);
      break;
    }
      
    case OP_DIVIDE_MON:
    {
      if (vm->popOne(&v1))
      {
        switch (v1->type)
        {
          case TYPE_LIST:
          {
            Value *o = NULL;
            sfold((List*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_DIVIDE)), vm);
            if (o)
              vm->push(o);
            break;
          }
          case TYPE_ARRAY:
          {
            Value *o = NULL;
            sfold((Array*)v1, &o, new CodeStandard(new OpcodeInstruction(OP_DIVIDE)), vm);
            if (o)
              vm->push(o);
            break;
          }
          default: break;
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
        if (vm->popOne(&v1) && v1->type == TYPE_INT)
        {
          vm->push(vm->lazy->at(vm, vm->lazy->index-((Int*)v1)->get()));
        }
      }
      else if (vm->popTwo(&v1, &v2))
      {
        switch (TYPES(v1->type, v2->type))
        {
          case TYPES(TYPE_LAMBDA, TYPE_LAMBDA):
          {
            Code *condition = ((Lambda*)v1)->get();
            Code *body = ((Lambda*)v2)->get();
            
            vm->execute(condition);
            
            while (vm->popOne(&v3) && v3->type == TYPE_BOOL && ((Bool*)v3)->get())
            {
              vm->execute(body);
              vm->execute(condition);
            }
            break;
          }
          case TYPES(TYPE_INT, TYPE_LAMBDA):
          {
            Code *c = ((Lambda*)v2)->get();
            int j = ((Int*)v1)->get();
            
            for (int i = 0; i < j; ++i)
            {
              vm->execute(c);
            }
            break;
          }
          case TYPES(TYPE_LIST, TYPE_LAMBDA):
          {
            list<Value*>* v = ((List*)v1)->get();
            Code *c = ((Lambda*)v2)->get();
            list<Value*>::iterator it;
            
            list<Value*>* ot = new list<Value*>();
            list<Value*>* of = new list<Value*>();
            
            for (it = v->begin(); it != v->end(); ++it)
            {
              vm->push(*it);
              vm->execute(c);
              
              if (vm->popOne(&v3))
              {
                if (v3->type == TYPE_BOOL && ((Bool*)v3)->get())
                  ot->push_back(*it);
                else
                  of->push_back(*it);
              }
            }
            
            vm->push(new List(of));
            vm->push(new List(ot));
            break;
            
          }
          case TYPES(TYPE_ARRAY, TYPE_LAMBDA):
          {
            vector<Value*>* v = ((Array*)v1)->get();
            Code *c = ((Lambda*)v2)->get();
            vector<Value*>::iterator it;
            
            vector<Value*>* ot = new vector<Value*>();
            vector<Value*>* of = new vector<Value*>();
            
            for (it = v->begin(); it != v->end(); ++it)
            {
              vm->push(*it);
              vm->execute(c);
              
              if (vm->popOne(&v3))
              {
                if (v3->type == TYPE_BOOL && ((Bool*)v3)->get())
                  ot->push_back(*it);
                else
                  of->push_back(*it);
              }
            }
            
            vm->push(new Array(of));
            vm->push(new Array(ot));
            break;
            
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_LAMBDA):
          {
            u32 s = 0;
            LazyArray *lazy = (LazyArray*)v1;
            Lambda *lambda = (Lambda*)v2;
            bool finished = false;
            
            do {
              Value *v = lazy->get().at(vm, s);
              vm->push(v);
              vm->execute(lambda->get());
              
              if (vm->popOne(&v3))
              {
                if (v3->type != TYPE_BOOL || !((Bool*)v3)->get())
                  finished = true;
              }
              
              ++s;
              
            } while (!finished);
            
            lazy->get().shrinkBy(1);
            vm->push(v1);
            
            break;
          }
          case TYPES(TYPE_LAZY_ARRAY, TYPE_INT):
          {
            LazyArray *lazy = (LazyArray*)v1;
            int i = ((Int*)v2)->get();
            
            for (int j = lazy->size(); j < i; ++j)
              lazy->get().at(vm, j);

            vm->push(v1);
            
            break;
          }
        }
      }
      break;
    }
      
    case OP_DBANG:
    {
      if (vm->popTwo(&v1, &v2))
      {
        if (v1->type == TYPE_LAMBDA)
        {
          //vm->push(new Lambda(new CurriedCode(((Lambda*)v1)->get(), v2)));
          break;
        }
      }
      break;
    }
      
    case OP_RAISE_STACK: { vm->raiseStack(); break; }
    case OP_LOWER_STACK: { vm->lowerStack(); break; }
      
    case OP_RAISE_STACKV:
    { 
      if (vm->popOne(&v1) && v1->type == TYPE_INT)
      {
        int t = ((Int*)v1)->get();
        
        if (vm->stackHasValues(t))
        {
          list<Value*> tmpStack;
          
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
      if (vm->popOne(&v1) && v1->type == TYPE_INT)
      {
        int t = ((Int*)v1)->get();
        
        if (vm->stackHasValues(t))
        {
          list<Value*> tmpStack;
          
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
      if (vm->popOne(&v1))
      {
        vm->push(new String(v1->svalue()));
      }
    }

  }
}




Code* InstructionTree::assemble()
{
  Code *code = new CodeStandard(depth);
  
  InstructionTree *tree = this;
  for (int i = 0; i < depth; ++i, tree = tree->n)
    code->set(depth-i-1, tree->i);
  
  return code;
}