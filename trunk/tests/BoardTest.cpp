#include <tut/tut.hpp>

#include "board.h"

namespace tut 
{ 
    struct board_test_data // initialized before each test
    {
    };

    test_group<board_test_data> testsuite("Board() tester");
    typedef test_group<board_test_data>::object testcase;

    template<> 
    template<>
    void testcase::test<1>()
    { 
        set_test_name("getSide() test");

        Board b;

        b.generate(1, 0);
        ensure_equals(b.getSide(), 1);

        b.generate(4, 0);
        ensure_equals(b.getSide(), 4);

        b.generate(8, 52);
        ensure_equals(b.getSide(), 8);
    }

    template<>
    template<>
    void testcase::test<2>()
    {
        set_test_name("click() test");

        Board b;

        b.generate(3, 0);
        ensure_equals(b.getSide(), 3);
        ensure_equals(b.getBlackCount(), 0);
        ensure_equals(b.getWhiteCount(), 3*3);
        ensure_equals(b.isWon(), false);

        // click the x to solve the 3x3 board
        //  012
        //0 x.x
        //1 .x.
        //2 x.x

        b.click(1, 1);
        ensure_equals(b.getBlackCount(), 5);
        ensure_equals(b.getWhiteCount(), 4);
        ensure_equals(b.isWon(), false);

        b.click(0, 0);
        ensure_equals(b.getBlackCount(), 4);
        ensure_equals(b.getWhiteCount(), 5);
        ensure_equals(b.isWon(), false);

        b.click(2, 0);
        ensure_equals(b.getBlackCount(), 5);
        ensure_equals(b.getWhiteCount(), 4);
        ensure_equals(b.isWon(), false);

        b.click(2, 2);
        ensure_equals(b.getBlackCount(), 6);
        ensure_equals(b.getWhiteCount(), 3);
        ensure_equals(b.isWon(), false);

        b.click(0, 2);
        ensure_equals(b.getBlackCount(), 9);
        ensure_equals(b.getWhiteCount(), 0);
        ensure_equals(b.isWon(), true);
    }

    template<>
    template<>
    void testcase::test<3>()
    {
        set_test_name("retry() test");

        Board b;

        b.generate(8, 50);
        ensure_equals(b.getSide(), 8);

        long black = b.getBlackCount();
        long white = b.getWhiteCount();

        //  01234567
        //0 ........
        //1 ........
        //2 ..x...x.
        //3 ........
        //4 ....x...
        //5 ........
        //6 ..x...x.
        //7 ........

        b.click(2, 2);
        b.click(2, 6);
        b.click(6, 2);
        b.click(6, 6);
        b.click(4, 4);

        ensure(black != b.getBlackCount());
        ensure(white != b.getWhiteCount());

        b.retry();
        ensure_equals(b.getBlackCount(), black);
        ensure_equals(b.getWhiteCount(), white);
    }
}
