#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <unordered_map>
#include <boost/optional/optional.hpp>

using namespace std;
using boost::optional;

//  This code is organized in bottom up fashion: first the math
//  utilities, then the distance class ('dist'), then the classes 'car'
//  and 'parking_space' , then finally the 'parking_lot' class

//  First, some functions to compare IEEE double precision numbers
//

//  IEEE double has 52 bits of mantissa.  This gives 15 digits.  
//  Don't see any constants in math.h that specify this.
const int MAX_DBL_SIG_FIG=15;

bool double_equal( double a, double b, 
                   unsigned int sig_figs = MAX_DBL_SIG_FIG )  
{
  int exponent = -min( (unsigned int)MAX_DBL_SIG_FIG, sig_figs );
  double allowable_delta = a * pow( 10.0d, exponent );
  return ( abs(a-b) < allowable_delta );
}

bool double_greater( double a, double b, 
                     unsigned int sig_figs = MAX_DBL_SIG_FIG )
{
  return ( a > b && !double_equal(a,b,sig_figs) );
}


//  *** instances denote a distance, including units of measurement
//
class dist {
public:
  enum dst_units { CM = 0, INCH = 1, FOOT = 2, METER = 3, YARD = 4 };
  dist( dst_units u, double d ) : units(u),distance(d) {};
  double get_dist( dst_units u ) const { return distance*conversion(units, u); };
  void set_dist( dst_units u, double d )  { units = u; distance = d; };
  //  bool is_greater ( const dist &  other_distance ) const  {
  //    return double_greater(distance, other_distance.get_dist(units)); };
  static double conversion( dst_units from, dst_units to );
private:
  dst_units units;
  double    distance;
};

bool is_greater( const dist & a, const dist & b )  {
  return double_greater( a.get_dist(dist::CM), b.get_dist(dist::CM) );
};

constexpr static double ft = 2.54*12.0;
constexpr static double yd = ft * 3.0;
static map<dist::dst_units, double> conversions = 
  {  {dist::CM, 1.0}
   , {dist::INCH,2.54}
   , {dist::FOOT,ft}
   , {dist::METER,100.0}
   , {dist::YARD, yd} };

double dist::conversion( dst_units from, dst_units to )  
{
  return conversions[from] / conversions[to];
}


//  *** instances represent a car, within the context of a parking lot
//  _for our purposes here_, this can be a POD struct, so it is
//
struct car {
  dist width, length, height;
  string license_plate;
};

//  *** instances represent a single uncovered parking space
//  also POD, for our purposes here
//
struct parking_space {
  dist width, length;    // uncovered, so no height
  char row;
  unsigned int number;
  optional<car> occupant;
};
const optional<car> no_car = optional<car>();

//  *** instances represent a single-level parking lot
//
class parking_lot {
public:
  parking_lot() {};
  void add_space( const dist& w, const dist& l, char row, unsigned int no );
  void add_to_row( const dist& w, const dist& l, char row,   // add multiple
                   unsigned int first, unsigned int count ); // spaces
  bool park_car( const car & veh );
  bool remove_car( const string & license );
  parking_space find_car( const string & license ) const;
  optional<car> find_occupant( char row, unsigned int no ) const;
  void print_report() const;
private:
  // stores all the parking spaces in a single row
  typedef map<char, vector<parking_space>> row_data;
  row_data spaces_by_row;
  // parking_space_ref allows us to efficiently point to entries in
  // the vector of row_data.  we can't use a direct pointer, because
  // the vector could be reallocated and moved 
  typedef pair<vector<parking_space>*,int> parking_space_ref;
  unordered_map<string, parking_space_ref>  license_plate_map;
  deque<parking_space_ref> available_spaces;
  static parking_space & get_space( parking_space_ref r ) 
    { return  (*get<0>(r))[get<1>(r)]; };
};

void parking_lot::add_space(const dist& w, const dist& l, char row, unsigned int no )
{
  add_to_row( w, l, row, no, 1 ); 
}

void parking_lot::add_to_row(const dist& w, const dist& l, char row, 
                             unsigned int first, unsigned int count )
{
  auto mypair = spaces_by_row.find(row);
  if ( spaces_by_row.end() == mypair )  {
    spaces_by_row[row] = vector<parking_space>();
    mypair = spaces_by_row.find(row);
  }
  vector<parking_space>& vec = get<1>(*mypair);
  for (unsigned int i=first; i<first+count; ++i)  {
    vec.push_back( parking_space{ w, l, row, i, no_car } ); 
    available_spaces.push_back( make_pair(&vec,vec.size()-1) );
  }
}

bool parking_lot::park_car( const car & veh )
{
  dist  l = veh.length;
  dist  w = veh.width;
  auto spaceref = find_if( available_spaces.begin(), available_spaces.end(), 
             [&l,&w, this] (parking_space_ref ref)  {
             parking_space &p = get_space(ref);
             return (   is_greater(p.length,l)
                     && is_greater(p.width,w)  ); });
  if ( available_spaces.end() != spaceref ) {
    auto & space = get_space( *spaceref );
    space.occupant = optional<car>(veh);
    license_plate_map[veh.license_plate] = *spaceref;
    available_spaces.erase(spaceref);
    return true;
  } else {
    return false;
  }
}


bool parking_lot::remove_car( const string & license )  
{
  auto entry = license_plate_map.find(license);
  if ( entry == license_plate_map.end() )  {
    return false;
  }
  parking_space &space = get_space(get<1>(*entry));
  space.occupant = no_car;
  license_plate_map.erase(license);
  available_spaces.push_back(get<1>(*entry));
  return true;
}


void parking_lot::print_report() const
{
  cout << endl << endl << "Parking Lot Report" << endl;
  for ( auto& mypair : spaces_by_row ) {
    cout << "  Row " << get<0>(mypair) << endl;
    for ( auto& space : get<1>(mypair) )  {
      cout << "    Space " << space.number << " : ";
      if ( !space.occupant )  {
        cout << "empty" << endl;
      }  else  {
        cout << space.occupant.get().license_plate << endl;
      }
    }
  }
  cout << endl;
}

optional<car> parking_lot::find_occupant( char row, unsigned int no ) const
{
  auto mypair = spaces_by_row.find(row);
  if ( spaces_by_row.end() != mypair )  {
    const vector<parking_space>& vec = get<1>(*mypair);
    for (auto& space : vec)  
      if ( space.number == no )
        return space.occupant;
  }
  return no_car;
}



int main() {
  parking_lot p;
  dist w1(dist::METER, 2.45);
  dist l1(dist::FOOT, 17.0);
  dist w2(dist::METER, 2.6);
  dist l2(dist::FOOT, 19.0);
  dist w3(dist::METER, 2.7);
  dist l3(dist::FOOT, 21.0);
  car c1{ w1, l2, w1, string("PINOTNV") };
  car c2{ w3, l2, w2, string("133ABD") };
  car c3{ w1, l1, w1, string("166ABC") };
  car c4{ w1, l2, w1, string("CU LTR") };

  p.add_space( w1, l1, 'A', 1 );
  p.add_space( w1, l1, 'A', 2 );
  p.add_to_row( w2, l2, 'B', 1, 4 );
  p.add_to_row( w3, l3, 'C', 11, 4 );

  cout << "Park 1 " <<  p.park_car(c1) << endl;
  cout << "Park 2 " <<  p.park_car(c2) << endl;
  cout << "Park 3 " <<  p.park_car(c3) << endl;
  p.park_car(c4);

  p.print_report();
  p.remove_car( "PINOTNV" );
  p.print_report();
}
