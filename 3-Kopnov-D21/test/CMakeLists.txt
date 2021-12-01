add_executable(test func.cpp)
target_link_libraries(test graph gtest gtest_main)

add_executable(load_test load_test.cpp)
target_link_libraries(load_test graph gtest gtest_main)
