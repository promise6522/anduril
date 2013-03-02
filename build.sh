#! /bin/bash

g++ fiber.cpp scheduler.cpp test.cpp

if [ $? == 0 ]; then
    echo ""
    echo ">>>>>>>>>> Run the test >>>>>>>>>>"
    ./a.out
fi
