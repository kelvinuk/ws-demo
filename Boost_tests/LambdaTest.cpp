#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN // in only one cpp file
#define BOOST_TEST_MODULE LambdaTestSuite
#include <boost/test/included/unit_test.hpp>

#include "../utils/my_memory_check.hpp"
#include "../utils/my_string_utils.hpp"
/*
#ifdef _DEBUG
#define BOOST_TEST_DETECT_MEMORY_LEAK 0
#endif
*/
BOOST_AUTO_TEST_SUITE(LambdaTestSuite)
    //boost::debug::detect_memory_leaks(true);
    //_CrtSetDbgFlag(0);
    BOOST_AUTO_TEST_CASE(LambdaTest)
    {
        //BOOST_CHECK_EQUAL(3, 3);

        BOOST_CHECK(1 + 1 == 2);
    }
    BOOST_AUTO_TEST_CASE(check_revString) {
        //static char str[256] = "BOOST_TEST_DETECT_MEMORY_LEAK=0";
        //putenv(str);

        throw_error_with_memory_leak();
        BOOST_TEST(revString("abcd") == "dcba");
        BOOST_TEST(revString("12345") == "54321");
        BOOST_TEST(revString("Even") == "nevE");

        // Add a failure case
        //BOOST_TEST(revString("Odd") == "DDO");
    }
BOOST_AUTO_TEST_SUITE_END()