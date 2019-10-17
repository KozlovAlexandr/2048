cmake_minimum_required(VERSION 3.5)

add_library(engine logic.cpp engine.cpp)
target_link_libraries(engine display_lib pthread)