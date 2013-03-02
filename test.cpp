#include <iostream>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "scheduler.h"

class Test {
public:
    Test(const std::string& name) : name_(name) {
    }

    void run() {
        std::cout << "before " << name_
            << "::yield()!" << std::endl;
        anduril::yield();
        std::cout << "after " << name_
            << "::yield()!" << std::endl;
    }

private:
    std::string name_;
};


int main()
{
    anduril::Scheduler scheduler;

    Test test1("test1");
    scheduler.schedule(boost::bind(&Test::run, &test1));

    Test test2("test2");
    scheduler.schedule(boost::bind(&Test::run, &test2));

    scheduler.run();

    //error to use this
    anduril::yield();
}
