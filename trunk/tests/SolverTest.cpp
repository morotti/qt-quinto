#include <tut/tut.hpp>

#include "board.h"
#include "solver.h"

namespace tut
{
    struct solver_test_data // initialized before each test
    {
    };

    test_group<solver_test_data> SolverTestSuite("Solver() tester");
    typedef test_group<solver_test_data>::object testcase;

    template<> 
    template<>
    void testcase::test<1>()
    { 
        set_test_name("constructor test");

        Solver s;
        ensure_equals(s.isSolved(), false);
    }

    template<>
    template<>
    void testcase::test<2>()
    {
        set_test_name("solve and reset test");

        Solver s;
        ensure_equals(s.isSolved(), false);

        Board b;
        b.generate(1, 0);
        s.solve(b.getBoard());
        ensure_equals(s.isSolved(), true);

        s.reset();
        ensure_equals(s.isSolved(), false);
    }

    template<>
    template<>
    void testcase::test<3>()
    {
        set_test_name("2x2 test");

        Board b;
        b.generate(2, 0);

        Solver s;
        int n = s.solve(b.getBoard());
        //ensure_equals(n, 1);

        ensure_equals(s.isSolved(), true);
        ensure_equals(s.count(), 4);

        s.flip(1, 1);
        ensure_equals(s.count(), 3);

        s.flip(1, 1);
        ensure_equals(s.count(), 4);
    }

    template<>
    template<>
    void testcase::test<4>()
    {
        set_test_name("3x3 test");

        Board b;

        b.generate(3, 0);
        // click the x to solve the 3x3 board
        //  012
        //0 x.x
        //1 .x.
        //2 x.x

        Solver s;
        int n = s.solve(b.getBoard());
        //ensure_equals(n, 1);

        ensure_equals(s.isSolved(), true);
        ensure_equals(s.count(), 5);

        s.flip(1, 1);
        ensure_equals(s.count(), 4);

        s.flip(0, 0);
        ensure_equals(s.count(), 3);

        s.flip(2, 2);
        ensure_equals(s.count(), 2);

        s.flip(2, 0);
        ensure_equals(s.count(), 1);

        s.flip(0, 2);
        ensure_equals(s.count(), 0);

        s.flip(0, 2);
        ensure_equals(s.count(), 1);
    }

}
