///  A lock manager that guarantees no deadlocks, if used by
///  different threads of a single process (unsafe to use if
///  locks are used by different *processes*)

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/optional/optional.hpp>

///  Manages locks for a multithreaded application.  Does not handle
///  multiple processes.
///
///  Limitations:
///     * you have to specify the order of lock acquisition for a thread
///       before that thread can make a lock.
///     * you cannot mix mutexes/semaphores/etc; you are restricted to one type
///
// HandleT only needs to support comparisons and operator <<
//
// LockT must support the following operations: lock(), unlock()
//
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
    //   hmm, what if we construct it with vertices that are
    //   pair<name,lock> where lock is any one of mutex/sema/etc.  ???

    threadHandle register_thread(digraph dependencies);
    // release_thread ????

    ///  the set of locks owned by a thread
    ///  maintained as a separate object to make sure they are all
    ///  unlocked if the thread throws an exception and unwinds the stack
    class lockSet  {

    };

    void lock(const threadHandle thread, const HandleT lock );
    bool try_lock( const threadHandle thread, const HandleT lock );
    void release(const threadHandle thread, const HandleT lock);
private:
    /// a helper class to make sure we unlock even in the face of exceptions
    class lockObj {
    public:
        lockObj(LockT &t) : _t(t), _locked(true) { _t.lock(); };
        ~lockObj() { Unlock(); };
        void Unlock() {
            if (_locked)  {
                _t.unlock();
                _locked = false;
            };
        };
    private:
        T &_t;
        bool _locked;
    };

    int next_handle = 0;
    graph lock_graph;
};
