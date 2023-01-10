cmake_minimum_required(VERSION 3.5)
project(eleos VERSION 0.1.0 LANGUAGES CXX)

add_library(eleos INTERFACE include/eleos.hpp)
target_include_directories(eleos
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)