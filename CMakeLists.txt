cmake_minimum_required(VERSION 3.22)
project(physray CXX)


set(exe_name "run")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp" "src/*.h" "lib/*.h")
include_directories(".")
include_directories("lib")
include_directories("src")

find_package(OpenMP REQUIRED)


add_executable(${exe_name} ${sources})

if(OpenMP_CXX_FOUND)
    target_compile_options(${exe_name} PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_libraries(${exe_name} PRIVATE  ${OpenMP_CXX_FLAGS})
endif()