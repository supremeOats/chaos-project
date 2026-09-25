cmake_minimum_required(VERSION 3.15)
project(ray_tracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp)

add_executable(raytracer
    src/main.cpp

    src/geometry/Vector3.cpp
    src/geometry/Hittable.cpp
    src/geometry/Mesh.cpp
    src/geometry/Sphere.cpp
    src/geometry/Moveable.cpp
    
    src/renderer/Camera.cpp
    src/renderer/Renderer.cpp
    
    src/acceleration/BVH.cpp
    
    src/utils/Color.cpp
    src/utils/Parser.cpp
    src/utils/ImageBuffer.cpp
    src/utils/MeshUtils.cpp

    include/utils/Matrix.h
    src/acceleration/BoundingBox.cpp
)

add_executable(raytrace_animation
    src/demo_animation.cpp

    src/geometry/Vector3.cpp
    src/geometry/Hittable.cpp
    src/geometry/Mesh.cpp
    src/geometry/Sphere.cpp
    src/geometry/Moveable.cpp
    
    src/renderer/Camera.cpp
    src/renderer/Renderer.cpp
    
    src/acceleration/BVH.cpp
    
    src/utils/Color.cpp
    src/utils/Parser.cpp
    src/utils/ImageBuffer.cpp
    src/utils/MeshUtils.cpp

    include/utils/Matrix.h
)

target_include_directories(raytracer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_include_directories(raytrace_animation PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(raytracer PRIVATE $<$<CONFIG:Debug>:DEBUG>)

if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /WX)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Winit-self -Werror)
endif()
