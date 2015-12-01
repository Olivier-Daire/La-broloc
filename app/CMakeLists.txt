file(GLOB HEADER_FILES include/*.hpp)
file(GLOB SRC_FILES src/*.cpp)

set(EXECUTABLE "La-broloc")

add_executable(${EXECUTABLE} ${SRC_FILES} ${HEADER_FILES} main.cpp)
target_link_libraries(${EXECUTABLE} ${ALL_LIBRARIES})
