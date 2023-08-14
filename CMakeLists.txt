cmake_minimum_required(VERSION 3.5.0)
project(trees VERSION 0.0.1 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
)
add_executable(trees ${SOURCES})

add_subdirectory(${CMAKE_SOURCE_DIR}/SFML)
include_directories(${CMAKE_SOURCE_DIR}/SFML/include)
link_directories(${CMAKE_SOURCE_DIR}/SFML/lib)

target_link_libraries(trees sfml-graphics sfml-window sfml-system)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)