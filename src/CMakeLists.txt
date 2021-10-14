find_package(pybind11 CONFIG REQUIRED)
find_package(libics REQUIRED)

pybind11_add_module(pyics)
target_include_directories(pyics PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(pyics PRIVATE cxx_std_20)
target_link_libraries(pyics PRIVATE ics::ics)

add_subdirectory(pyics)
