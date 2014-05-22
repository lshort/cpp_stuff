#include <stdexcept>
#include <memory>
#include <ostream>
#include <iostream>
#include <fstream>
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

const int MAX_DBL_SIG_FIG = numeric_limits<double>::digits - 1;

/**  Compares two IEEE double values for equality, to a specific
     number of significant figures
     @param[in] a the first comparison value
     @param[in] b the second comparison value
     @return true if the two values are equal, or very close to it */
bool double_equal( double a, double b,
                   unsigned int sig_figs = MAX_DBL_SIG_FIG )
{
  int exponent = -min( (unsigned int)MAX_DBL_SIG_FIG, sig_figs );
  double allowable_delta = a * pow( 10.0, exponent );
  return ( abs(a-b) < allowable_delta );
}

bool double_greater( double a, double b,
                     unsigned int sig_figs = MAX_DBL_SIG_FIG )
{
  return ( a > b && !double_equal(a,b,sig_figs) );
}


/// instances denote a distance, including units of measurement
///
class dist {
public:
  enum dst_units { CM = 0, INCH = 1, FOOT = 2, METER = 3, YARD = 4 };
  dist( dst_units u, double d ) : units(u),distance(d) {};
  double get_dist( dst_units u ) const { return distance*conversion(units, u); };
  void set_dist( dst_units u, double d )  { units = u; distance = d; };
  //  bool is_greater ( const dist  &other_distance ) const  {
  //    return double_greater(distance, other_distance.get_dist(units)); };
  static double conversion( dst_units from, dst_units to );
private:
  dst_units units;
  double    distance;
};

bool is_greater( const dist &a, const dist &b )  {
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

///  instances represent a single-level parking lot
///
class parking_lot {
public:
  parking_lot() {};
  parking_lot(const parking_lot &orig);   // make a deep copy
  parking_lot(const parking_lot &&orig) ;
  friend void swap( parking_lot& first, parking_lot& second );
  parking_lot &operator = (parking_lot orig);
  void add_space( const dist &w, const dist &l,
                  const char row, const unsigned int no );
  void add_to_row( const dist &w, const dist &l, const char row,   // add multiple
                   const unsigned int first, const unsigned int count ); // spaces
  bool park_car( const car &veh );
  bool remove_car( const string &license );
  optional<parking_space> find_car( const string &license ) const;
  optional<car> find_occupant( char row, unsigned int no ) const;
private:
  // stores all the parking spaces in a single row
  typedef map<char, vector<parking_space>> row_data;
  row_data spaces_by_row;
  // parking_space_ref allows us to efficiently point to entries in
  // the vector of row_data.  we can't use a direct pointer to the
  // individual parking_spaces, the vector could be reallocated and
  // moved  and then the individual pointers would be invalid
  typedef pair<vector<parking_space>*,int> parking_space_ref;
  unordered_map<string, parking_space_ref>  license_plate_map;
  deque<parking_space_ref> available_spaces;
  static parking_space &get_space( const parking_space_ref &r )
    { return  (*get<0>(r))[get<1>(r)]; };
  parking_space_ref copy_ref( const parking_space_ref &r );
  friend ostream & operator << (ostream & out, const parking_lot &p);
};



/**  Print out the argument to the specified ostream
     @param[in] p The parking_space to print */
ostream & operator << (ostream & out, const parking_lot &p ) {
    out << "[";
    auto &last_row_pair = *(--p.spaces_by_row.end());
    for ( auto &mypair : p.spaces_by_row ) {
        out << "{ Row " << get<0>(mypair) ;
        for ( auto &space : get<1>(mypair) )  {
            out << "( " << space.number << "<"
                << space.length.get_dist(dist::FOOT)
                << "\'," << space.width.get_dist(dist::FOOT) << "\'>: ";
            if ( !space.occupant )  {
                out << "empty )";
            }  else  {
                out << space.occupant->license_plate << " )";
            }
        }
        out << "}";
        if (&mypair==&last_row_pair)
            out << "]";
        out << endl;
    }
    return out;
}


/** Populate parking_lot data from stream input
    @param[out] p The parking lot to populate   */
istream & operator >> (istream & in, parking_lot & p)  {
    char d; // used to read past delimiters
    string row, license;
    char row_name, next;
    int no;
    double w, l;
    in >> d;    // read the opening '['
    while (!in.eof())  {
        if (']'==in.peek())  {
            in >> d;
        } else {
            // read { >> 'Row'  >> row_name >> (
            in >> d >> row >> row_name >> next;
            while (next != '}')  {
                // read space# >> '<' >> len >> ' >> , >> wid >> ' >> '>' >> ':'
                in >> no >> d >> l >> d >> d >> w >> d >> d >> d;
                // read lic# >> ) >> next delimiter, either ( or }
                in >> license >> d >> next;
                p.add_space( dist(dist::FOOT, w), dist(dist::FOOT,l), row_name, no );
                // set occupant -- issue: need more info to construct a car()
            }
        }
    }
    in >> d;
#ifdef DEBUG
    cout << p;
#endif
}

/** A function to swap two parking_lot objects
    @param[in,out] first The first parking_lot to swap
    @param[in,out] second The second parking lot to swap */
void swap( parking_lot& first, parking_lot& second ) {
    using std::swap;
    std::swap( first.spaces_by_row, second.spaces_by_row );
    std::swap( first.license_plate_map, second.license_plate_map );
    std::swap( first.available_spaces, second.available_spaces );
}

/** The move constructor
    @param[in] orig The parking_lot which is assigned from */
parking_lot::parking_lot(const parking_lot &&orig) {
  spaces_by_row     = move(orig.spaces_by_row);
  license_plate_map = move(orig.license_plate_map);
  available_spaces  = move(orig.available_spaces);
}

/** The parameter 'r' points to a parking_space_ref in a different
    parking_lot.  We build and return a parking_space_ref in our own
    parking_lot that points to the parking_space that has the same
    row and number as 'r' points to.  Needed for the copy constructor.
    @param[in] r The reference that we are copying
    @return The reference that we have built  */
parking_lot::parking_space_ref parking_lot::copy_ref( const parking_space_ref &r ) {
  vector<parking_space> *oldvec = get<0>(r);
  int index = get<1>(r);
  parking_space &space = (*oldvec)[index];
  vector<parking_space> *newvec = &spaces_by_row[space.row];
  return make_pair( newvec, index );
}

/** The copy constructor, makes a deep copy
    @param[in] orig The parking_lot which is assigned from */
parking_lot::parking_lot(const parking_lot &orig) {
  // get our own copy of the map and vectors of data it contains
  spaces_by_row = orig.spaces_by_row;
  for ( auto &entry : orig.license_plate_map )  {
    parking_space_ref spaceref = get<1>(entry);
    license_plate_map[get<0>(entry)] = copy_ref( spaceref );
  }
  for ( auto &spaceref : orig.available_spaces ) {
    available_spaces.push_back(copy_ref(spaceref));
  }
}

/** The assignment operator
    @param[in] orig The parking_lot which is assigned from
    @return Returns the value to be assigned */
parking_lot &parking_lot::operator = (parking_lot orig) {
  swap(*this, orig);
  return *this;
}

/** Adds a single space to the row
    @param[in] w The width of the parking space
    @param[in] l The length of the parking space
    @param[in] row The row's identifying letter
    @param[in] no The number of the parking space */
void parking_lot::add_space(const dist &w, const dist &l, char row, unsigned int no )
{
  add_to_row( w, l, row, no, 1 );
}

/** Adds several parking spaces to the row, numbering them sequentially
    @param[in] w The width of the parking spaces
    @param[in] l The length of the parking spaces
    @param[in] row The row's identifying letter
    @param[in] first The number of the first parking space
    @param[in] count The number of parking spaces to add   */
void parking_lot::add_to_row(const dist &w, const dist &l, char row,
                             unsigned int first, unsigned int count )
{
  auto mypair = spaces_by_row.find(row);
  if ( spaces_by_row.end() == mypair )  {
    spaces_by_row[row] = vector<parking_space>();
    mypair = spaces_by_row.find(row);
  }
  vector<parking_space> &vec = get<1>(*mypair);
  for (unsigned int i=first; i<first+count; ++i)  {
    vec.push_back( parking_space{ w, l, row, i, no_car } );
    available_spaces.push_back( make_pair(&vec,vec.size()-1) );
  }
}

/** Parks the car in an appropriately sized parking space
    @param[in] veh The car
    @return Returns false if no space was available, true otherwise */
bool parking_lot::park_car( const car &veh )
{
  dist  l = veh.length;
  dist  w = veh.width;
  auto spaceref = find_if( available_spaces.begin(), available_spaces.end(),
             [&l,&w, this] (parking_space_ref ref)  {
             parking_space &p = get_space(ref);
             return (   is_greater(p.length,l)
                     && is_greater(p.width,w)  ); });
  if ( available_spaces.end() != spaceref ) {
    auto &space = get_space( *spaceref );
    space.occupant = optional<car>(veh);
    license_plate_map[veh.license_plate] = *spaceref;
    available_spaces.erase(spaceref);
    return true;
  } else {
    return false;
  }
}

/** Removes the car with the specified license plate
    @param[in] license The license plate to search for
    @return Returns true if it found a matching car, false otherwise */
bool parking_lot::remove_car( const string &license )
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


/**  Find the car the occupies a specific parking space
     @param[in] row  The row of the parking space
     @param[in] no   The number of the parking space
     @return If not found -- the empty object
     @return If found -- the car data */
optional<car> parking_lot::find_occupant( char row, unsigned int no ) const
{
  auto mypair = spaces_by_row.find(row);
  if ( spaces_by_row.end() != mypair )  {
    const vector<parking_space> &vec = get<1>(*mypair);
    for (auto &space : vec)
      if ( space.number == no )
        return space.occupant;
  }
  return no_car;
}

/**  Find a car with the specified license plate
     @param [in] license The license plate to find
     @return If not found -- the empty object
     @return If found -- the parking space data */
optional<parking_space> parking_lot::find_car( const string &license ) const {
  try {
    const parking_space_ref &ref = license_plate_map.at(license);
    vector<parking_space> *vec = get<0>(ref);
    int index = get<1>(ref);
    return optional<parking_space>((*vec)[index]);
  } catch (const std::out_of_range & out) {
    return optional<parking_space>();
  }
}



int main(int argc, char *argv[]) {

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

    parking_lot p;
    if (argc==1) {
        p.add_space( w1, l1, 'A', 1 );
        p.add_space( w1, l1, 'A', 2 );
        p.add_to_row( w2, l2, 'B', 1, 4 );
        p.add_to_row( w3, l3, 'C', 11, 4 );
    } else {
        ifstream f(argv[1]);
        f >> p;
    }


  cout << "Park 1 " <<  p.park_car(c1) << endl;
  cout << "Park 2 " <<  p.park_car(c2) << endl;
  cout << "Park 3 " <<  p.park_car(c3) << endl;
  p.park_car(c4);

  cout << p;
  p.remove_car( "PINOTNV" );
  cout << p;
  p.find_car("CU LTR");
  p.find_occupant('C',12);
}
