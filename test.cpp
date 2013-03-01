#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "fiber.h"

void callback()
{
    std::cout << "Print from callback!" << std::endl;
}


int main()
{
    boost::shared_ptr<anduril::Fiber> pfb =
        boost::make_shared<anduril::Fiber>(callback);

    anduril::Scheduler scheduler;
    scheduler.schedule(pfb);
    scheduler.run();
}
