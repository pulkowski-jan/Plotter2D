cmake_minimum_required(VERSION 3.25)
project(plotter2d)


file(GLOB SRC_FILES
        "${CMAKE_SOURCE_DIR}/*.cpp"
        "${CMAKE_SOURCE_DIR}/model/*.cpp"
        "${CMAKE_SOURCE_DIR}/visualization/*.cpp"
        "${CMAKE_SOURCE_DIR}/evaluation/*.cpp"
        "${CMAKE_SOURCE_DIR}/parser/*.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

set(CMAKE_CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} sfml-window)
target_link_libraries(${PROJECT_NAME} sfml-system)
