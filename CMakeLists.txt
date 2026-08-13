cmake_minimum_required(VERSION 3.15)
project(ray_tracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp)

add_executable(myproject
    src/main.cpp

    src/geometry/Vector3.cpp
    src/geometry/Mesh.cpp
    src/geometry/Moveable.cpp
    
    src/renderer/Camera.cpp
    src/renderer/Renderer.cpp
    src/renderer/shading/ShadingFunctions.cpp
    
    src/utils/Color.cpp
    src/utils/Parser.cpp

    include/utils/Matrix.h
)

target_include_directories(myproject PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
