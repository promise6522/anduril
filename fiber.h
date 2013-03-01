#ifndef __ANDURIL_FIBER_H_

#include <list>

#include <boost/function.hpp>

namespace anduril{

class Scheduler;

class Fiber {

    friend class Scheduler;
public:
    typedef boost::shared_ptr<Fiber> ptr;

    Fiber(boost::function<void()> cb) : cb_(cb) {

    }

    //schedule(const boost::shared_ptr<Scheduler>& scheduler);
private:

    //invoke by the scheduler only
    void schedule_() {
        cb_();
    }

private:
    boost::function<void()> cb_;

};

class Scheduler {
public:
    Scheduler() {};

    void run() {
        while (!run_queue_.empty()) {
            //TODO sync
            Fiber::ptr pfb = run_queue_.front();
            run_queue_.pop_front();

            pfb->schedule_();
        }
    }

    void schedule(const boost::shared_ptr<Fiber>& pfb) {
        //TODO sync
        run_queue_.push_back(pfb);
    }

    void yield();

private:
    std::list<Fiber::ptr> run_queue_;

};

}

#endif
