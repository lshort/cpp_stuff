//  security.h -- declares the Security and GrowthBurst classes
//     used for processing securities data and finding the best
//     periods of growth for each

#ifndef SECURITY_H__
#define SECURITY_H__

#include <deque>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;


class Security;

//  GrowthBurst -- represents a Security's gains over a time period
//     note: immutable once constructed
class GrowthBurst {
public:
  GrowthBurst( const Security & secty, double delta, int start, int end ) :
     security(secty), value_ratio(delta), start_time(start), end_time(end) {};
  string getStr() const;
  double getGain() const;
  friend class SortBursts;
private:
  GrowthBurst();
  const Security & security;
  double value_ratio;
  int start_time;
  int end_time;
};


// compares two GrowthBursts; given how the std lib's sort() is implemented,
// will result in a sorted list with highest growth first
class SortBursts
{
public:
  bool operator() (const GrowthBurst* const l, const GrowthBurst* const r) const
          {  return (l->value_ratio > r->value_ratio); };
};


//  Security -- represents the data we have for a security
//      note: immutable once constructed
class Security {
public:
  Security( const string & name_val, const string & ticker_val,
            const deque<double> & price_listings ) :
       prices(price_listings), name(name_val), ticker(ticker_val) {};
  GrowthBurst* getBestBurst() const;
  // getBestBurst calculates highest-growth burst of any duration on an absolute
  // (rather than per unit time) basis
  const string& getName() const;
  const string& getTicker() const;
  double getPrice( const unsigned int time ) const;
  // getPrice returns signaling_NaN if out of range
private:
  Security();
  deque<double> prices;
  string name;
  string ticker;
};


#endif
