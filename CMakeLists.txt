cmake_minimum_required(VERSION 3.5)
project(rssh-client)

find_package(Threads REQUIRED)
find_package(Catch2 REQUIRED)

add_subdirectory(src)
# if(BUILD_TESTING)
add_subdirectory(tests)
# endif()

