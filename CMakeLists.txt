cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME_STR git)

project(${PROJECT_NAME_STR})
target_link_libraries(${CMAKE_CURRENT_SOURSE_DIR}/include/*.h)
file(GLOB SOURCE_PATH ${CMAKE_CURRENT_SOURSE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME_STR} ${SOURCE_PATH})