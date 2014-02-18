#include "security.h"
#include <unit++.h>
#include <string>
#include <deque>
#include <math.h>

using namespace unitpp;
using namespace std;

bool fp_eql( const double a, const double b )
{
  if ( fabs(a) < .0001 )
    return ( fabs(b) < .0001 );
  else
    return ( (a-b)/b < .0001 );
}

namespace
{
  class SecurityTest : public suite
  {

    // tests constructor, getters
    void testBasics()
    {
      const string nm("Name-A");
      const string tc("TCKA");
      const deque <double> prc(3,10.0);
      const Security sec( nm, tc, prc );
      assert_eq( "Get Name",0,nm.compare(sec.getName()) );
      assert_eq( "Get Ticker", 0, tc.compare(sec.getTicker()) );
      assert_eq( "Get Price 0",true, fp_eql(10.0, sec.getPrice(0)) );
    };

    // helper fcn for testSingleBurst
    Security* buildSec( const double* const data, const int count,
                        const char* name, const char* tick )
    {
      deque <double> prc;
      for (int i=0; i<count; i++)
        prc.push_back( data[i] );
      return  (new Security( name, tick, prc ) );
    }

    // tests a single getBestBurst; called by testBursts
    void testSingleBurst(const bool expect_success, const double expected_val,
                         const double* const data, const int count)
    {
      static int test_count = 0;
      char test_name[32];
      snprintf( test_name, 31, "Test Burst-%02d", ++test_count );
      test_name[31] = '\0';
      const Security* sec = buildSec( data, count, "A Name", "ATCK" );
      if (expect_success)
        assert_eq( test_name, true,
                   fp_eql(expected_val, sec->getBestBurst()->getGain()) );
      else
        assert_eq( test_name, true, NULL == sec->getBestBurst() );
      free(sec);
    }

    // tests getBestBurst
    void testBursts()
    {
      double data1[] = {4.0, 5.0, 5.0, 5.0, 4.0, 6.0};
      testSingleBurst(true, 0.0, data1+1, 2);
      testSingleBurst(true, 0.0, data1+1, 3);
      testSingleBurst(true, .25, data1, 2);
      testSingleBurst(true, .25, data1, 5);
      testSingleBurst(true, .5, data1, 6);
      testSingleBurst(true, 0.0, data1+2, 3);
      testSingleBurst(true, .20, data1+3, 3);
      testSingleBurst(true, -.20, data1+3, 2);
      testSingleBurst(false, 1.0, data1+3, 1);
      testSingleBurst(false, 1.0, data1+3, 0);
      double data2[] = {8.0, 10.0, 9.0, 11.0, 12.0};
      testSingleBurst(true, -0.1, data2+1, 2);
      testSingleBurst(true, 0.1, data2+1, 3);
      testSingleBurst(true, .25, data2, 2);
      testSingleBurst(true, .20, data2, 5);
    };

    // tests SortBursts
    void testSort() {
      deque<Security*> securities;
      deque<GrowthBurst*> bursts;
      double data1[] = {4.0, 5.0, 5.0, 5.0, 4.0, 6.0};
      securities.push_back( buildSec( data1+1, 2, "N+00", "Name+00") );
      securities.push_back( buildSec( data1, 6, "N+50", "Name+50") );
      securities.push_back( buildSec( data1, 2, "N+25", "Name+25") );
      securities.push_back( buildSec( data1+3, 3, "N+20", "Name+20") );
      securities.push_back( buildSec( data1+3, 2, "N-20", "Name-20") );
      double data2[] = {8.0, 10.0, 9.0, 11.0, 12.0};
      securities.push_back( buildSec( data2+1, 2, "N-10", "Name-10") );
      securities.push_back( buildSec( data2+1, 3, "N+10", "Name+10") );
      for ( deque<Security*>::const_iterator s=securities.begin();
            s != securities.end(); ++s )
         bursts.push_back((*s)->getBestBurst());
      sort( bursts.begin(), bursts.end(), SortBursts() );
      assert_eq( "Sort 1", true, fp_eql(0.5,bursts.at(0)->getGain()));
      assert_eq( "Sort 2", true, fp_eql(.25,bursts.at(1)->getGain()));
      assert_eq( "Sort 3", true, fp_eql(.20,bursts.at(2)->getGain()));
      assert_eq( "Sort 4", true, fp_eql(0.10,bursts.at(3)->getGain()));
      assert_eq( "Sort 5", true, fp_eql(0.0,bursts.at(4)->getGain()));
      assert_eq( "Sort 6", true, fp_eql(-.1,bursts.at(5)->getGain()));
      assert_eq( "Sort 7", true, fp_eql(-.2,bursts.at(6)->getGain()));
    }

  public:
    SecurityTest() : suite("Security class test")
    {
      add("Test Basic Functions", testcase(this, "TestBasics", &SecurityTest::testBasics));
      add("Test Bursts", testcase(this, "TestBursts", &SecurityTest::testBursts));
      add("Test Sort", testcase(this, "TestSort", &SecurityTest::testSort));
      suite::main().add("TestSecurity", this);
    }
  };
  SecurityTest * secTest = new SecurityTest();
}
