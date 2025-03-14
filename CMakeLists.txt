cmake_minimum_required(VERSION 3.30)
project(plotter2d)

set(CMAKE_CXX_STANDARD 17)

add_executable(plotter2d main.cpp model/plot_model.cpp visualization/visualization.cpp)

target_compile_features(plotter2d PRIVATE cxx_std_17)
target_link_libraries(plotter2d sfml-graphics)
target_link_libraries(plotter2d sfml-window)
target_link_libraries(plotter2d sfml-system)
