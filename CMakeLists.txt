cmake_minimum_required(VERSION 3.28...3.29)
project(ws_demo)

find_package( Boost 1.83...<1.86 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_STANDARD 17)

add_executable(ws_demo main.cpp
        utils/my_logger.cpp
        utils/my_logger.hpp
        utils/my_lambda_func.cpp
        utils/my_string_utils.cpp
        utils/my_string_utils.hpp
        utils/my_memory_check.hpp
        utils/my_memory_check.cpp
        ipc/boost_mo_ws_client_async.cpp
        ipc/boost_mo_ws_client_async.hpp
        ipc/boost_https_login.cpp
        ipc/boost_https_login.hpp
        utils/my_json_parser.cpp
        utils/my_logger.hpp
        ipc/event_handler.cpp
        ipc/event_handler.hpp)

target_link_libraries( ws_demo LINK_PUBLIC ${Boost_LIBRARIES} crypto ssl boost_json)
add_subdirectory(Boost_tests)