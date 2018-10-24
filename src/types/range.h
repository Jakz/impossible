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
#include <numeric>

struct RangePair
{
  integral_t a, b;
  RangePair(integral_t a, integral_t b) : a(a), b(b) { }
};

class RangeVector
{
public:
  using data_t = std::vector<RangePair>;
  
public:
  data_t data;
  
  RangeVector(const data_t& data) : data(data) { }
  RangeVector() { }
  RangeVector(integral_t a, integral_t b)
  {
    if (b < a)
      std::swap(a, b);
    
    data.push_back(RangePair(a,b));
  }
  
  data_t::iterator begin() { return data.begin(); }
  data_t::iterator end() { return data.end(); }
  data_t::const_iterator begin() const { return data.begin(); }
  data_t::const_iterator end() const { return data.end(); }
  
  const RangePair& operator[](const size_t index) const { return data[index]; }
  
  //TODO: data type int?
  std::vector<integral_t> concretize() const
  {
    std::vector<integral_t> v;
    v.reserve(size());
    
    for (size_t i = 0; i < data.size(); ++i)
    {
      RangePair rp = data.at(i);
      for (integral_t j = rp.a; j <= rp.b; ++j)
        v.push_back(j);
    }
    
    return v;
  }
  
  bool empty() const { return data.empty(); }
  
  integral_t size() const
  {
    return std::accumulate(
      data.begin(),
      data.end(),
      0LL,
      [] (integral_t v, const RangePair& p) { return v + p.b - p.a + 1; }
    );
  }
  
  RangeVector merge(integral_t v) const
  {
    const data_t& d = data;
    RangeVector res;
    
    if (v < d.front().a - 1)
    {
      res.data.push_back(RangePair(v,v));
      res.data.insert(res.data.end(), d.begin(), d.end());
    }
    else if (v == d.front().a - 1)
    {
      res.data.push_back(RangePair(v,d.front().b));
      res.data.insert(res.data.end(), d.begin()+1, d.end());
    }
    else if (v > d.back().b + 1)
    {
      res.data.insert(res.data.begin(), d.begin(), d.end());
      res.data.push_back(RangePair(v,v));
    }
    else if (v == d.back().b + 1)
    {
      res.data.insert(res.data.begin(), d.begin(), d.end()-1);
      res.data.push_back(RangePair(d.back().a,v));
    }
    else
    {
      
    }
    
    return res;
  }
  
  RangeVector rangeUnion(const RangeVector& r2) const
  {
    const data_t& v1 = data;
    const data_t& v2 = r2.data;
    
    std::list<RangePair> s;
    
    data_t::const_iterator it1, it2;
    
    for (it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end() || it2 != v2.end(); )
    {
      if (it1 != v1.end() && it2 != v2.end())
      {
        const RangePair &rp1 = *it1, &rp2 = *it2;
        
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
      else if (it1 != v1.end())
      {
        s.insert(s.end(), it1, v1.end());
        break;
      }
      else if (it2 != v2.end())
      {
        s.insert(s.end(), it2, v2.end());
        break;
      }
    }
    
    data_t o;
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
          o.push_back(rp);
          rp = cp;
        }
      }
    }
    
    o.push_back(rp);
    
    return RangeVector(o);
  }  
};

#endif
