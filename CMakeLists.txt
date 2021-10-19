cmake_minimum_required(VERSION 3.17)
project(PyIcs VERSION 0.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(pybind11 CONFIG REQUIRED)
find_package(Ics REQUIRED)

add_subdirectory(src)
