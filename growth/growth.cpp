//  growth.cpp -- a standalone program that reads in data from a securities file
//     and then calculates the best growth period for each security and outputs
//     a sorted list of the results

#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "security.h"


//  strips (in-place) leading/trailing spaces and leading [s and trailing ]s
//      value...the string to be stripped
void strip_padding( string& value )
{
  while (value.length()>0 && (' '==value.at(0) || '['==value.at(0)) )
     value.erase(0,1);
  while (value.length()>0
         &&  (' ' == value.at(value.length()-1)  || ']' == value.at(value.length()-1)) )
     value.erase(value.length()-1,1);
}

// reads the input file and builds from it Security objects
// which is returned via a deque of points
//      filename...the name of the file with the securities data
deque<Security*> load_data_file(const string& filename )
{
  deque<Security*> retval;
  deque<double> price_listings;
  string name, ticker, prices, single_price;

  ifstream infile(filename.c_str());
  if (infile.is_open())
    while (!infile.eof()) {
      getline( infile, name, '|' );
      // check for last line with no content
      if (name=="")
        continue;
      strip_padding(name);
      getline( infile, ticker, '|');
      strip_padding(ticker);
      getline( infile, prices );
      strip_padding(prices);
      istringstream pricestream(prices);
      while (getline(pricestream, single_price, ','))  {
        strip_padding(single_price);
        price_listings.push_back( stoi(single_price) );
      }
      retval.push_back( new Security( name, ticker, price_listings ) );
      price_listings.clear();
    }
  return retval;
}

//  loads the securities data from file, calculates the top
//  growth burst for each security, and prints out the results
//  from the highest to the lowest, to the stream specified
//    filename....the name of the file with the securities data
//    outstr....the output stream
void process_securities_file(const char* filename, ostream& outstr)
{
  deque<GrowthBurst*> bursts;
  deque<Security*> securities = load_data_file( filename );
  for ( const auto &s : securities )
    bursts.push_back((*s).getBestBurst());
  sort( bursts.begin(), bursts.end(), SortBursts() );
  for ( const auto &g : bursts )
     outstr <<  (*g).getStr() << endl;

  for ( auto &g : bursts )
    delete g;
  for ( auto &s : securities )
    delete s;
}


#ifndef TEST_HARNESS
int main( int argc, char* argv[] )
{
  if (argc < 2) {
    cout << "Usage: growth <datafile>" << endl;
    return 0;
  }
  process_securities_file( argv[1], cout );
  return 1;
}
#endif
