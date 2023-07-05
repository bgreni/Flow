
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(Lexer)
add_subdirectory(Parser)
add_subdirectory(TestCommon)
# include(GoogleTests)
# gtest_discover_tests(declaration_test)