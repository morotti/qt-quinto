#include <tut/tut.hpp>

#include <set>

namespace tut 
{ 
    struct example_test_data // initialized before each test
    { 
    };
    
    test_group<example_test_data> ExampleTestSuite("example tester");
    typedef test_group<example_test_data>::object testcase;

    template<> 
    template<>
    void testcase::test<1>()
    { 
        set_test_name("size() test");

        std::set<std::string> s;
        s.insert("aaa");
        s.insert("bbb");
        s.insert("ccc");

        ensure_equals(s.size(), 3);

        s.clear();
        ensure_equals(s.size(), 0);
    }
}
