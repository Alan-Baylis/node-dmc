cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME dmc)
project(${PROJECT_NAME})

# Include BoostLib module
file(GLOB_RECURSE boostlib_cmake_path "${CMAKE_CURRENT_SOURCE_DIR}/node_modules" "BoostLib.cmake")
list(GET boostlib_cmake_path 0 boostlib_cmake_path)
get_filename_component(boostlib_cmake_path "${boostlib_cmake_path}" DIRECTORY)
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${boostlib_cmake_path}")
include(BoostLib)

message (STATUS ${Eigen3_DIR})

include_directories(${Boost_INCLUDE_DIRS})

find_package (Eigen3 REQUIRED NO_MODULE PATHS ${Eigen3_DIR} NO_DEFAULT_PATH)
include_directories(EIGEN3_INCLUDE_DIR)

file(GLOB SOURCE_FILES "include/dmc/*.cc" "include/dmc/*.hpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_INC};${Boost_LIBRARIES};Eigen3::Eigen)


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
