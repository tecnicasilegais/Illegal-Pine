cmake_minimum_required(VERSION 3.19)
project(Pine_Shooter)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB IMAGE "image_libs/*")
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "headers/*.h")

add_executable(Pine_Shooter ${SOURCES} ${IMAGE} ${HEADERS} main.cpp src/Engine.cpp headers/Engine.h)
target_link_libraries(Pine_Shooter -lglut -lGLU -lGL -lSOIL)