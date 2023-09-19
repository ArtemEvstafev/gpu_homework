cmake_minimum_required(VERSION 3.11)git@gitlab.com:learngpu/materials.git
project(firstblood)

add_subdirectory(../utils utils)

add_executable(${PROJECT_NAME} main_rhombus.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)
