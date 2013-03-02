#include "scheduler.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "fiber.h"

namespace anduril {
namespace detail {

// Current running sheduler (per thread)
__thread Scheduler*         g_scheduler;

}

Scheduler* getScheduler() {
    return detail::g_scheduler;
}

void yield() {
    if (detail::g_scheduler)
        detail::g_scheduler->yieldBack();
    else
        printf("can't call yield() : no scheduler is running!\n");
}


Scheduler::Scheduler() : stop_(false) {

}

void Scheduler::run() {
    detail::g_scheduler = this;

    while (!stop_) {
        if (run_queue_.empty())
            break;

        run_fiber_ = run_queue_.front();
        run_queue_.pop_front();

        //resume fiber execution
        swapcontext(&ctx_, &run_fiber_->ctx_);

        if (run_fiber_->exit_)
            run_fiber_->sched_ = NULL;
        else
            run_queue_.push_back(run_fiber_);

        run_fiber_.reset();
    }

    printf("Scheduler::run() exits\n");

    detail::g_scheduler = NULL;
}

void Scheduler::schedule(boost::function<void()> callback) {
    // create a new fiber
    Fiber::ptr fb = boost::make_shared<Fiber>(callback);

    fb->sched_ = this;

    run_queue_.push_back(fb);
}

void Scheduler::yieldBack() {
    assert(run_fiber_);
    swapcontext(&run_fiber_->ctx_, &ctx_);
}

}
