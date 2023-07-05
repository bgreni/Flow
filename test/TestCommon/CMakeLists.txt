project(TestCommon)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.h" "*.cpp")

target_link_libraries(${PROJECT_NAME} Token)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${SRC}
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include)