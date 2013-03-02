#ifndef __ANDURIL_SCHEDULER_H_

#include <ucontext.h>

#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace anduril{

class Fiber;
typedef boost::shared_ptr<Fiber> FiberPtr;

class Scheduler {
public:
    Scheduler();

    void run();

    void schedule(boost::function<void()> callback);

    void stop();

private:
    friend void yield();
    void yieldBack();

private:

    bool                        stop_;
    ucontext_t                  ctx_;
    FiberPtr                    run_fiber_;
    std::list<FiberPtr>         run_queue_;
};

// get the scheduler the fiber is scheduled on
Scheduler* getScheduler();

// called within a fiber context to yield to the current scheduler
void yield();

}

#endif
