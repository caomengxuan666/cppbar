add_executable(test_progress test_progress.cpp)
target_link_libraries(test_progress PRIVATE cppbar)

add_test(NAME progress_test COMMAND test_progress)
