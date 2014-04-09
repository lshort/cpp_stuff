///  A lock manager that guarantees no deadlocks, if used by
///  different threads of a single process (unsafe to use if
///  locks are used by different *processes*)

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>


// HandleT only needs to support comparisons and operator <<
// LockT must support the following operations:
template <typename HandleT, typename LockT>
class lockManager {
public:
    // default constructor is generated
    // copies of the manager can introduce deadlocks, so disallow copying
    lockManager(const lockManager & toCopy) = delete;
    lockManager &operator = (const lockManager &toCopy) = delete;
    // default move constructor is generated
    // default destructor is generated

    typedef int threadHandle;
    const threadHandle invalid_handle = -1;
    typedef boost::adjacency_list<boost::vecS, boost::vecS,
                                  boost::directedS, HandleT> digraph;

    threadHandle register_thread(digraph<HandleT> dependencies);
    // release_thread ????
    void lock(const threadHandle thread, const HandleT lock );
    bool try_lock( const threadHandle thread, const HandleT lock );
    void release(const threadHandle thread, const HandleT lock);
private:
    int next_handle = 0;
    graph lock_graph
};
