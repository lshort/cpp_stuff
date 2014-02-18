#include <unit++.h>
#include <string>
#include <deque>
#include <math.h>

#define TEST_HARNESS
#include "growth.cpp"

using namespace unitpp;
using namespace std;


namespace
{
  class HelperFcnTests : public suite
  {
    // called by testProcessFile, validates the the two
    // files have identical content
    void testFileEq( const char* f1, const char* f2 )
    {
      const int buflen=8096;
      char data1[buflen], data2[buflen];
      ifstream str1(f1);
      ifstream str2(f2);
      if (!str1.is_open())
        assert_eq( "Stream 1 Open Failed", true, false );
      if (!str2.is_open())
        assert_eq( "Stream 2 Open Failed", true, false );

      str1.read(data1,buflen);
      str2.read(data2,buflen);
      if (!str1.eof() || !str2.eof())
        assert_eq("Stream Buffer Too Short", true, false );
      assert_eq("Different Data Lengths",true,str1.gcount()==str2.gcount());
      for (int i=0; i<str1.gcount(); ++i )
        assert_eq("Different Data", true, data1[i]==data2[i]);
    }

    // test process_securities_file, accomplished by comparing
    // the actual output file with the expected output
    void testProcessFile()
    {
      const char* outfile = "data2_out.txt";
      const char* infile = "data2.txt";
      ofstream outfilestrm(outfile);
      if (!outfilestrm.is_open())
        assert_eq("Output Stream Failed to open",false,true);
      process_securities_file( infile, outfilestrm );
      outfilestrm.close();
      testFileEq(outfile, "data2_expected.out" );
    }

    // tests strip_padding by having sets of prefixes/base values/suffixes
    // which are both good and bad.  For any combination of good values,
    // the stripped result should be the same as the base value.  If exactly
    // one of the (prefix|base value|suffix) is bad, then the stripped result
    // should be different from the base value.  strip_padding is tested on
    // all combinations which have at most one bad value
    void testStrip()
    {
      const char* good_prefixes[]= {" ", "   ", "[", "[ [ [", "  [ ", "[[" };
      deque<string> good_pre(good_prefixes, good_prefixes+6);

      const char* good_bases[]= {"abc","","1","3.14","!@","A5","|||","4[]5","! #"};
      deque<string> good_bas(good_bases, good_bases+9);

      const char* good_suffixes[]= {" ", "   ", "]", "] ] ]", "  ] ", "]]" };
      deque<string> good_suf(good_suffixes, good_suffixes+6);

      const char* bad_prefixes[]= {"a", " ! ", "9", "[ $ [", "  K ", "0[" };
      deque<string> bad_pre( bad_prefixes, bad_prefixes+6);

      const char* bad_bases[]= {"ab]"," ","]","[.14","! ","[["," | ","[4]5","!# "};
      deque<string> bad_bas( bad_bases, bad_bases+9);

      const char* bad_suffixes[]= {"a", " ! ", "6", "] [ ]", "  I ", "@]" };
      deque<string> bad_suf(bad_suffixes, bad_suffixes+6);

      for ( const auto &pre : good_pre )
          for ( const auto &bas : good_bas )
              for ( const auto &suf : good_suf )
            {
              string test_str( pre + bas + suf );
              strip_padding(test_str);
              assert_eq(test_str,true,0==test_str.compare(bas));
            }
      for ( const auto &pre : bad_pre )
          for ( const auto &bas : good_bas )
              for ( const auto &suf : good_suf )
              {
                  string test_str( pre + bas + suf );
                  strip_padding(test_str);
                  assert_eq(test_str,false,0==test_str.compare(bas));
              }
      for ( const auto &pre : good_pre )
          for ( const auto &bas : bad_bas )
              for ( const auto &suf : good_suf )
              {
                  string test_str( pre + bas + suf );
                  strip_padding(test_str);
                  assert_eq(test_str,false,0==test_str.compare(bas));
              }
      for ( const auto &pre : good_pre )
          for ( const auto &bas : good_bas )
              for ( const auto &suf : bad_suf )
              {
                  string test_str( pre + bas + suf );
                  strip_padding(test_str);
                  assert_eq(test_str,false,0==test_str.compare(bas));
              }
    };

  public:
    HelperFcnTests() : suite("helper function tests")
    {
      add("Test Strip Padding", testcase(this, "TestStrip", &HelperFcnTests::testStrip));
      add("Test File Processing", testcase(this, "TestFileProc", &HelperFcnTests::testProcessFile));
      suite::main().add("TestPadding", this);
    }
  };
  HelperFcnTests * hlpTest = new HelperFcnTests();
}
