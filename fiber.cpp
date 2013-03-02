#include "fiber.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace anduril {

extern void yield();

Fiber::Fiber(boost::function<void()> cb) : cb_(cb), exit_(false) {
    assert(0 == getcontext(&ctx_));

    size_t ssize = 16 * 1024;// 16k stack enough?
    ctx_sp_ = ::malloc(ssize);
    assert(ctx_sp_);

    ctx_.uc_stack.ss_sp = ctx_sp_;
    ctx_.uc_stack.ss_size = ssize;
    ctx_.uc_link = NULL;//TODO

    typedef void (*func_type) ();
    ::makecontext(&ctx_, (func_type)&Fiber::start, 1, this);
}

Fiber::~Fiber() {
    printf("~Fiber::Fiber\n");

    ::free(ctx_.uc_stack.ss_sp);
}

void Fiber::start() {
    cb_();
    exit_ = true;
    yield();
}

}

