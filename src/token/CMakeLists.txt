project(Token)

add_library(${PROJECT_NAME} STATIC)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.h" "*.cpp")

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${SRC}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include)