These are some helper template files.  

container_stream.hpp has the stream output operator << for most containers (both regular and associative).  

expect_exception.hpp has a template that helps with testing functions that are expected to throw in failure conditions.  C++14/1y is required.

vector_subrange.hpp has a template that lets you treat a subrange of a vector as a vector in its own right (ie, you can get iterators from it and you can use it in a range based for)

