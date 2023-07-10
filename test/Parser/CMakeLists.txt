project(ParserTests)
include(GoogleTest)

foreach(X IN ITEMS Parser_StatementTests Parser_ExpressionTests Parser_Compound)
    add_executable(${X} ${X}.cpp)
    target_link_libraries(${X} Lexer Parser GTest::gtest_main fmt::fmt TestCommon ${llvm_libs})    
    gtest_discover_tests(${X})
endforeach()