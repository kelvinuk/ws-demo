set(Boost_DEBUG ON)
set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

add_executable (Boost_Tests_run LambdaTest.cpp
        ../utils/my_string_utils.cpp
        ../utils/my_memory_check.cpp)
target_link_libraries (Boost_Tests_run ${Boost_LIBRARIES})