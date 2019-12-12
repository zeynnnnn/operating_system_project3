cmake_minimum_required(VERSION 3.15)
project(operating_system_project3 C)

set(CMAKE_C_STANDARD 99)
add_executable(operating_system_project3 app.c)
add_library (test_library STATIC memalloc.c memalloc.h)
find_library(TEST_LIBRARY test_library lib)
target_link_libraries(operating_system_project3  LINK_PUBLIC ${TEST_LIBRARY} m rt pthread)
