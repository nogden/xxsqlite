add_executable(test_database
    test_database.cpp
)
target_link_libraries(test_database
    sqlite
    gtest
    gtest_main
)
add_test(test_database
    COMMAND test_database
)
