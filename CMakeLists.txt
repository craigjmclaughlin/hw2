cmake_minimum_required(VERSION 3.17)
project(CS3258_cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DGL_SILENCE_DEPRECATION")

add_executable(CS3258_cmake main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(CS3258_cmake ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}) # Link libraries.