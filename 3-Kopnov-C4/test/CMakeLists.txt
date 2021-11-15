add_executable(submission submission.c)
add_executable(load_test load_test.cpp)
target_link_libraries(load_test graph gtest gtest_main)
