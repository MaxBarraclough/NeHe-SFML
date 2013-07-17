## This file is based on the CMake example given at
## https://github.com/LaurentGomila/SFML/wiki/Tutorial%3A-Build-your-SFML-project-with-CMake

## Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(myproject)

## Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
## (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

## Set version information in a config.h file
set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

## Define sources and executable
set(EXECUTABLE_NAME_1  "nehe_lesson_01")
set(EXECUTABLE_NAME_2  "nehe_lesson_02")
set(EXECUTABLE_NAME_3  "nehe_lesson_03")
set(EXECUTABLE_NAME_4  "nehe_lesson_04")
set(EXECUTABLE_NAME_5  "nehe_lesson_05")
set(EXECUTABLE_NAME_6  "nehe_lesson_06")
set(EXECUTABLE_NAME_7  "nehe_lesson_07")
set(EXECUTABLE_NAME_8  "nehe_lesson_08")
set(EXECUTABLE_NAME_9  "nehe_lesson_09")
set(EXECUTABLE_NAME_10 "nehe_lesson_10")
set(EXECUTABLE_NAME_11 "nehe_lesson_11")
set(EXECUTABLE_NAME_12 "nehe_lesson_12")
set(EXECUTABLE_NAME_45 "nehe_lesson_45")

add_executable(${EXECUTABLE_NAME_1}  nehe_lesson_01.cpp)
add_executable(${EXECUTABLE_NAME_2}  nehe_lesson_02.cpp)
add_executable(${EXECUTABLE_NAME_3}  nehe_lesson_03.cpp)
add_executable(${EXECUTABLE_NAME_4}  nehe_lesson_04.cpp)
add_executable(${EXECUTABLE_NAME_5}  nehe_lesson_05.cpp)
add_executable(${EXECUTABLE_NAME_6}  nehe_lesson_06.cpp)
add_executable(${EXECUTABLE_NAME_7}  nehe_lesson_07.cpp)
add_executable(${EXECUTABLE_NAME_8}  nehe_lesson_08.cpp)
add_executable(${EXECUTABLE_NAME_9}  nehe_lesson_09.cpp)
add_executable(${EXECUTABLE_NAME_10} nehe_lesson_10.cpp)
add_executable(${EXECUTABLE_NAME_11} nehe_lesson_11.cpp)
add_executable(${EXECUTABLE_NAME_12} nehe_lesson_12.cpp)
add_executable(${EXECUTABLE_NAME_45} nehe_lesson_45.cpp)


## Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
## Find any version 2.X of SFML
## See FindSFML.cmake for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME_1}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_2}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_3}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_4}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_5}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_6}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_7}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_8}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_9}  ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_10} ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_11} ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_12} ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_45} ${SFML_LIBRARIES})
endif()


## Install target
install(TARGETS ${EXECUTABLE_NAME_1}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_2}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_3}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_4}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_5}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_6}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_7}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_8}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_9}  DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_10} DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_11} DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_12} DESTINATION bin)
install(TARGETS ${EXECUTABLE_NAME_45} DESTINATION bin)


## CPack packaging
include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${EXECUTABLE_NAME_1}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_2}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_3}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_4}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_5}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_6}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_7}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_8}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_9}  ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_10} ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_11} ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_12} ${OPENGL_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME_45} ${OPENGL_LIBRARIES})

  ## or: target_link_libraries(${EXECUTABLE_NAME_1} ${OPENGL_gl_LIBRARY})
#  target_link_libraries(${EXECUTABLE_NAME_1}  m)  # if you use maths.h
#  .....
endif()


## Installers should include the 'data' directory, which should be placed
## at the same level as the output executables.
## This isn't ideal (it's certainly not the Unix-ey way to do things),
## but at least it gets the files included in the installer.

install (FILES "${PROJECT_SOURCE_DIR}/data/NeHe.bmp"
               "${PROJECT_SOURCE_DIR}/data/cube.bmp"
               "${PROJECT_SOURCE_DIR}/data/glass.bmp"
               "${PROJECT_SOURCE_DIR}/data/mud.bmp"
               "${PROJECT_SOURCE_DIR}/data/star.bmp"
               "${PROJECT_SOURCE_DIR}/data/tim.bmp"
               "${PROJECT_SOURCE_DIR}/data/world.txt"

         DESTINATION bin/data)

