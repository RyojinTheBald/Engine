cmake_minimum_required(VERSION 3.9.6)
project(engine)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ")

set(CMAKE_PREFIX_PATH "E:/Ryojin/dev/vcpkg/installed/x64-windows")

#find_package(PkgConfig)
#pkg_check_modules(GLM REQUIRED glm)
#pkg_check_modules(GLEW REQUIRED glew)
#pkg_check_modules(GLFW REQUIRED glfw3)

find_package(Threads)
find_package(Assimp CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Engine ${SOURCES})

target_include_directories(Engine PUBLIC
    lib/entt/src
    lib/cpp-taskflow
    lib/glm
    lib/glfw-3.3.2.bin.WIN64/include
    lib/glew-2.1.0/include
)

target_link_libraries(Engine 
    ../lib/glew-2.1.0/lib/Release/x64/glew32s
    ../lib/glfw-3.3.2.bin.WIN64/lib-vc2019/glfw3
    assimp::assimp
    ${CMAKE_THREAD_LIBS_INIT}
)

target_compile_options(Engine PRIVATE
    -DGLFW_INCLUDE_GLU
    -DGLEW_STATIC
    /std:c++17
)

target_link_options(Engine PRIVATE
    /NODEFAULTLIB:library
)
