///  expect_exception.hpp
///      Some code to help in writing lightweight tests.  Allows you to 
///      specify which tests should throw and which should not.  
///
///      At first, it will seem strange that all 4 of the arguments
///      are needed (if you don't expect to throw, you can never
///      call exc_lambda).  But it is so much easier to compose lambdas
///      into tests this way -- look at the examples, you'll see.  

#include <iostream>

/**   This code calls a lambda.  If the result is expected to throw and 
      throws, it calls a different lambda.  Similarly if it is expected 
      not to throw and doesn't throw.  But if it is expect to throw and 
      doesn't, or vice versa, then it throws.  Got that?
      @param[in] exe_lambda The test code to run; a lambda
      @param[in] expect_p Do we expect this execution to throw
      @param[in] exc_lambda Executed if expect_p is True and the
                 exe_lambda threw
      @param[in] no_exc_lambda Executed if expect_p is False and the
                 exe_lambda didn't throw.  The return value of exe_lambda
                 is passed as an argument to this lambda  
      @return The result of exc_lambda or no_exc_lambda, whichever runs
      */
template<typename ExeLambda, typename OnExcLambda, typename NoExcLambda>
auto expect_exception( ExeLambda exe_lambda, bool expect_to_throw,
                       OnExcLambda exc_lambda, NoExcLambda no_exc_lambda)
{
    bool threw;
    decltype( exe_lambda() ) x;

    try {
        x = exe_lambda();
        threw = false;
    }
    catch (...) {
        threw = true;
    }

    if ( expect_to_throw != threw ) {
        if (expect_to_throw)
            throw "failed to catch expected exception";
        else
            throw "caught unexpect exception";
    } else {
        if (expect_to_throw)
            return exc_lambda();
        else
            return no_exc_lambda(x);
    }
}


/**  
     template<typename ExeLambda, typename AllowExcLambda>
     auto expect_exception( ExeLambda exe_lambda, AllowExcLambda except_lambda )
     //                       OnExcLambda on_exc )
     {
     try {
     auto x = exe_lambda();
     if (except_lambda())
     throw "failed to catch expected exception";
     return x;
     }
     catch (...) {
     if (!except_lambda())
     throw "caught unexpect exception";
     auto y;
     return y;
     }
     }
*/

