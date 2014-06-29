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
      @param[in] exec_lambda The test code to execute; a lambda.
      @param[in] expect_to_throw True if we expect to throw.
      @param[in] throw_lambda Executed if expect_to_throw is True and the
                 exec_lambda threw.  
      @param[in] no_throw_lambda Executed if expect_to_throw is False and
                 the exec_lambda didn't throw.  The return value of 
                 exec_lambda is passed as an argument to this lambda.
      @return The result of exc_lambda or no_exc_lambda, whichever runs
      */
template<typename ExecLambda, typename OnThrowLambda, typename NoThrowLambda>
auto expect_exception( ExecLambda exec_lambda, bool expect_to_throw,
                       OnThrowLambda throw_lambda, NoThrowLambda no_throw_lambda,
                       decltype(throw_lambda()) expected_return_value)
{
    bool threw;
    decltype( exec_lambda() ) x;
    
    try {
        x = exec_lambda();
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
        if (expect_to_throw) {
            return throw_lambda();
        } else {
            auto rval = no_throw_lambda(x);
            if ( rval != expected_return_value )
                throw "Unexpected Return Value!!!";
            else
                return rval;
        }
    }
}


template<typename ExecLambda, typename ExpectThrowLambda,
         typename OnThrowLambda, typename NoThrowLambda>
auto expect_exception_l( ExecLambda exec_lambda,
                         ExpectThrowLambda expect_throw,
                         OnThrowLambda throw_lambda,
                         NoThrowLambda no_throw_lambda,
                         decltype(throw_lambda()) expect)
{
    return expect_exception(exec_lambda, expect_throw(),
                            throw_lambda, no_throw_lambda, expect);
}

