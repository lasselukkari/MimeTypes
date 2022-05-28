cmake_minimum_required(VERSION 3.1)

set(project_name mime-types)

project(${project_name}-exe C CXX)

file(GLOB_RECURSE SRC
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.ino
    ${CMAKE_CURRENT_LIST_DIR}/*.hpp
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

## add executables
add_executable(${project_name}-exe ${SRC})

target_link_libraries(${project_name}-exe
    PRIVATE ${project_name}-lib)

add_dependencies(${project_name}-exe ${project_name}-lib)