include_directories(include/)
file(GLOB_RECURSE SRC_FILES *.cpp *.hpp)

set(EXECUTABLE "La-broloc")
add_executable(${EXECUTABLE} ${SRC_FILES} main.cpp)
target_link_libraries(${EXECUTABLE} ${ALL_LIBRARIES})
