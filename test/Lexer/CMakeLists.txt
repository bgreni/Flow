Project(LexerTests)

include(GoogleTest)
foreach(X IN ITEMS DeclarationTests ReturnTests OperatorTests CompoundTests)
    add_executable(${X} ${X}.cpp)
    target_link_libraries(${X} Lexer GTest::gtest_main fmt::fmt TestCommon Codegen)
    gtest_discover_tests(${X})
endforeach()
