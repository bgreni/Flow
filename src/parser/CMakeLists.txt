project(Parser)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.h" "*.cpp")

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${SRC}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    # where the library itself will look for its internal headers
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Parser
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)

target_link_libraries(${PROJECT_NAME} Token Lexer Ast Common)