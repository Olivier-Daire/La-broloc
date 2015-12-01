cmake_minimum_required(VERSION 2.8)

project(ImacGL)

add_definitions(-std=c++11 -Wall -O2 -lassimp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

#assimp
include_directories(third-party/assimp/include)
# Override Assimp default behavior
set(ENABLE_BOOST_WORKAROUND ON CACHE BOOL "If a simple implementation of the used Boost functions is used. Slightly reduces functionality, but enables builds without Boost available.")
add_subdirectory(third-party/assimp)
# Prevent assimp warnings
set_target_properties(assimp PROPERTIES COMPILE_FLAGS "-w")

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} glimac/include third-party)

set(ALL_LIBRARIES glimac ${SDL2_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} assimp)

add_subdirectory(glimac)
add_subdirectory(app)
add_subdirectory(assets)