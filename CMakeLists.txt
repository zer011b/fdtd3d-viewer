cmake_minimum_required (VERSION 3.0.2)

project (fdtd3d-viewer)

set(FPTYPE "d" CACHE STRING "Defines type of floating point values")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORCE_INLINES")

add_definitions (-DVERSION="0.0.1")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set (CMAKE_BUILD_TYPE "Release")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
  add_definitions (-DENABLE_ASSERTS="")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "ReleaseWithAsserts")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
  add_definitions (-DENABLE_ASSERTS="")
endif ()

message ("C++ compiler: ${CMAKE_CXX_COMPILER}.")
message ("C compiler: ${CMAKE_C_COMPILER}.")

if ("${VALUE_TYPE}" STREQUAL "f")
  message ("Floating point values: float.")
  add_definitions (-DFLOAT_VALUES="")
elseif ("${VALUE_TYPE}" STREQUAL "d")
  message ("Floating point values: double.")
  add_definitions (-DDOUBLE_VALUES="")
elseif ("${VALUE_TYPE}" STREQUAL "ld")
  if ("${COMPLEX_FIELD_VALUES}")
    message(FATAL_ERROR "Complex long double values are not supported")
  endif ()

  message ("Floating point values: long double.")
  add_definitions (-DLONG_DOUBLE_VALUES="")
else ()
  message(FATAL_ERROR "Unknown values type")
endif ()

set (BUILD_FLAGS "")
set (BUILD_FLAGS "${BUILD_FLAGS} -std=c++11")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -flto -fno-fat-lto-objects")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -fno-stack-protector")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -fno-exceptions -fno-rtti")
endif ()

#-pedantic -pedantic-errors

set (WARNING_FLAGS "")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wfatal-errors")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wall -Wextra")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wformat=2 -Wformat-security -Wuninitialized")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Winit-self -Wno-stack-protector")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wconversion -Wsign-conversion")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wmissing-declarations")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set (BUILD_FLAGS "${BUILD_FLAGS} -O3")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "ReleaseWithAsserts")
  set (BUILD_FLAGS "${BUILD_FLAGS} -O1")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set (BUILD_FLAGS "${BUILD_FLAGS} -O0")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS} ${WARNING_FLAGS}")

###### Project ######
message ("Project dir: ${PROJECT_SOURCE_DIR}")
message ("Compile flags: ${CMAKE_CXX_FLAGS}")
message ("Link flags: ${LINK_FLAGS}")

add_subdirectory (Source)
