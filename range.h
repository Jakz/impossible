/*
 * Copyright (C) 2012-2014 Impossible Language
 * by Jacopo Santoni (Jack)
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 * Created on 1/2/13
 */

#ifndef _RANGE_H_
#define _RANGE_H_

#include <list>
#include <vector>

using namespace std;

struct RangePair
{
  int a, b;
  
  RangePair(int a, int b) : a(a), b(b) { }
};

class RangeVector
{
public:
  vector<RangePair> *data;
  
  RangeVector(vector<RangePair>* data) : data(data) { }
  
  RangeVector() : data(new vector<RangePair>()) { }
  
  RangeVector(int a, int b) : data(new vector<RangePair>())
  {
    if (b < a)
    {
      swap(a, b);
    }
    
    data->push_back(RangePair(a,b));
  }
  
  vector<int> concretize()
  {
    vector<int> v;
    v.reserve(size());
    
    for (int i = 0; i < data->size(); ++i)
    {
      RangePair rp = data->at(i);
      for (int j = rp.a; j <= rp.b; ++j)
        v.push_back(j);
    }
    
    return v;
  }
  
  u32 size() const
  {
    u32 size = 0;
    vector<RangePair> *d = data;
    
    for (int i = 0; i < d->size(); ++i)
      size += d->at(i).b - d->at(i).a + 1;
    
    return size;
  }
  
  RangeVector *merge(int v)
  {
    vector<RangePair> *d = data;
    RangeVector *res = new RangeVector();
    
    if (v < d->front().a - 1)
    {
      res->data->push_back(RangePair(v,v));
      res->data->insert(res->data->end(), d->begin(), d->end());
    }
    else if (v == d->front().a - 1)
    {
      res->data->push_back(RangePair(v,d->front().b));
      res->data->insert(res->data->end(), d->begin()+1, d->end());
    }
    else if (v > d->back().b + 1)
    {
      res->data->insert(res->data->begin(), d->begin(), d->end());
      res->data->push_back(RangePair(v,v));
    }
    else if (v == d->back().b + 1)
    {
      res->data->insert(res->data->begin(), d->begin(), d->end()-1);
      res->data->push_back(RangePair(d->back().a,v));
    }
    else
    {
      
    }
    
    return NULL;
  }
  
  RangeVector rangeUnion(RangeVector r2)
  {
    vector<RangePair>* v1 = data;
    vector<RangePair>* v2 = r2.data;
    
    list<RangePair> s;
    
    vector<RangePair>::iterator it1, it2;
    
    for (it1 = v1->begin(), it2 = v2->begin(); it1 != v1->end() || it2 != v2->end(); )
    {
      if (it1 != v1->end() && it2 != v2->end())
      {
        RangePair &rp1 = *it1, &rp2 = *it2;
        
        if (rp1.a < rp2.a)
        {
          s.push_back(rp1);
          ++it1;
        }
        else if (rp1.a > rp2.a)
        {
          s.push_back(rp2);
          ++it2;
        }
        else
        {
          if (rp1.b > rp2.b)
          {
            s.push_back(rp1);
            ++it1;
            ++it2;
          }
          else if (rp1.b < rp2.b)
          {
            s.push_back(rp2);
            ++it1;
            ++it2;
          }
          else
          {
            s.push_back(rp1);
            ++it1; ++it2;
          }
        } 
      }
      else if (it1 != v1->end())
      {
        s.insert(s.end(), it1, v1->end());
        break;
      }
      else if (it2 != v2->end())
      {
        s.insert(s.end(), it2, v2->end());
        break;
      }
    }
    
    vector<RangePair>* o = new vector<RangePair>();
    RangePair rp = s.front();
    s.pop_front();
    
    while (!s.empty())
    {
      RangePair cp = s.front();
      s.pop_front();
      
      if (cp.b <= rp.b)
        continue;
      else
      {
        if (cp.a <= rp.b + 1)
          rp.b = cp.b;
        else
        {
          o->push_back(rp);
          rp = cp;
        }
      }
    }
    
    o->push_back(rp);
    
    return RangeVector(o);
  }  
};

#endif
