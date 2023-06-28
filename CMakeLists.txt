cmake_minimum_required(VERSION 3.19)

project(
    Flow
    VERSION 1.0
    LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.0.0
)

FetchContent_MakeAvailable(fmt)

set(TARGET flowc)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} Lexer)


set(BUILD_TEST ON CACHE BOOL "Whether to build tests")
if (${BUILD_TEST})
    enable_testing()
    add_subdirectory(test)
endif()
