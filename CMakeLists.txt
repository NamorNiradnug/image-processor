cmake_minimum_required(VERSION 3.8)

project("Image Processor"
    VERSION 0.1
    DESCRIPTION "Simple image processor"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src/)
