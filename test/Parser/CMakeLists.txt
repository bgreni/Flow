project(ParserTests)
include(GoogleTest)

foreach(X IN ITEMS Parser_StatementTests)
    add_executable(${X} ${X}.cpp)
    target_link_libraries(${X} Lexer Parser GTest::gtest_main fmt::fmt TestCommon)    
    gtest_discover_tests(${X})
endforeach()