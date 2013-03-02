#ifndef __ANDURIL_FIBER_H_

#include <ucontext.h>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace anduril{

class Scheduler;

class Fiber {

public:
    typedef boost::shared_ptr<Fiber> ptr;

    Fiber(boost::function<void()> cb);

    ~Fiber();

private:
    // entry point for scheduling
    void start();

private:
    friend class Scheduler;

    bool                        exit_;
    boost::function<void()>     cb_;

    // current scheduled on
    Scheduler*                  sched_;

    // context and stack pointer
    ucontext_t                  ctx_;
    void*                       ctx_sp_;

};

}

#endif
