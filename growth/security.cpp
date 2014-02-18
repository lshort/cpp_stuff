//  security.h -- implements the Security and GrowthBurst classes
//     used for processing securities data and finding the best
//     periods of growth for each

#include "security.h"
#include <limits>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ********************************************************************
// implementation of Security
//

// gets the price at a given time
//   time....the time at which the price was recorded
double Security::getPrice( unsigned int time ) const
{
  if ( time >= prices.size() )
    return numeric_limits<double>::signaling_NaN();
  else
    return prices.at(time);
}

// finds the Security's highest percentage growth burst
//   looking for highest total growth, not growth per unit time
//   need not be continuous growth
//   if less than 2 prices, return NULL, as growth cannot be
//   measured even once
//   general strategy:  at any given time t, the highest growth
//   is equal to the max of (the highest previous growth) and
//   (the current price minus the lower price so far).  walk thru
//   the prices from earliest to latest and record as final the
//   highest growth when you reach the end
GrowthBurst* Security::getBestBurst() const
{
  if ( prices.size() < 2 )
    return NULL;

  int running_minimum = 0;
  double best = (prices.at(1) - prices.at(0))/prices.at(0);
  int start = 0;
  int end = 1;
  for ( unsigned int i=2; i<prices.size(); ++i )  {
    const double price_basis = prices.at(running_minimum);
    const double delta = (prices.at(i) - price_basis)/price_basis;
    if (delta > best)  {
      best = delta;
      end = i;
      start = running_minimum;
    };
    if (prices.at(i) < prices.at(running_minimum))
      running_minimum = i;
  }
  return  new GrowthBurst( *this, best, start, end );
}

const string& Security::getName() const
{
  return name;
}

const string& Security::getTicker() const
{
  return ticker;
}



// ********************************************************************
// implementation of GrowthBurst + friends
//

// get a printable string representing the burst
//
string GrowthBurst::getStr() const
{
  ostringstream outstr;
  outstr << value_ratio * 100 << "% from time " << start_time
         << " to " << end_time << " --- " << security.getTicker()
         << ": " << security.getName();
  return outstr.str();
}

// return the %age gain of the growth burst
//
double GrowthBurst::getGain() const
{
  return value_ratio;
}
