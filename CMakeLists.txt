cmake_minimum_required(VERSION 3.15)
project(ray_tracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp)

add_executable(myproject
    src/main.cpp
    src/renderer/Triangle.cpp
    src/renderer/Vector3.cpp
)

target_include_directories(myproject PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
