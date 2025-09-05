cmake_minimum_required(VERSION 3.20)

project(ray_tracing
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)


# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

# yaml-cpp
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)



file(GLOB SRC_FILES "src/*.cpp")
add_executable(ray_tracing main.cpp ${SRC_FILES})

target_include_directories(ray_tracing PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(ray_tracing PRIVATE STB_IMAGE_IMPLEMENTATION STB_IMAGE_WRITE_IMPLEMENTATION)


target_link_libraries(
    ray_tracing 
    PRIVATE
    spdlog::spdlog
    yaml-cpp::yaml-cpp
)

