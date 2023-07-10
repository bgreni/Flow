cmake_minimum_required(VERSION 3.20)

project(
    Flow
    VERSION 1.0
    LANGUAGES C CXX
)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

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
llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(${TARGET} Lexer Parser ${llvm_libs})


set(BUILD_TEST ON CACHE BOOL "Whether to build tests")
if (${BUILD_TEST})
    enable_testing()
    add_subdirectory(test)
endif()
