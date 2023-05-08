cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project(jetson_stats_spec)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}_example example.cpp)
