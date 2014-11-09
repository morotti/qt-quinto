#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>
#include <iostream>

using std::exception;
using std::cerr;
using std::endl;

namespace tut
{
    test_runner_singleton runner;
}

int main(int argc, char *argv[])
{
    tut::reporter reporter;
    tut::runner.get().set_callback(&reporter);

    tut::runner.get().run_tests();

    return !reporter.all_ok();
}
