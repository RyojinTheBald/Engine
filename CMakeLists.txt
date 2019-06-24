cmake_minimum_required(VERSION 3.9.6)
project(engine)

set(CMAKE_BUILD_TYPE Debug)

set(ENTT_INCLUDE_DIRS "lib/entt/src")
set(TASKFLOW_INCLUDE_DIRS "lib/cpp-taskflow")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -o3 -DGLFW_INCLUDE_GLU -DGLEW_STATIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")


find_package(PkgConfig)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(GLFW REQUIRED glfw3)

find_package(Threads)


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main.o ${SOURCES})

target_include_directories(main.o PRIVATE ${ENTT_INCLUDE_DIRS} ${TASKFLOW_INCLUDE_DIRS})

target_link_libraries(main.o ${GLM_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(main.o PUBLIC ${GLM_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} )
target_compile_options(main.o PUBLIC ${GLM_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER} ${GLFW_CFLAGS_OTHER})
