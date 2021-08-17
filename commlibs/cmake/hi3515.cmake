message(STATUS "CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")
if(CMAKE_CROSSCOMPILING)
  return()  # in case re-enter toolchain file
endif()

# set(CROSS_A "arm-linux" CACHE STRING "gnu compiler triple")
# message(STATUS "${CROSS_A}")

set(CMAKE_SYSTEM_NAME    Linux) # Target system name
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR HI3515)

set(CROSS_COMPILER_PATH "/opt/hisi-linux/x86-arm/gcc-3.4.3-uClibc-0.9.28/usr/bin")
set(CROSS_COMPILER "arm-hismall-linux-")
set(CMAKE_C_COMPILER "${CROSS_COMPILER_PATH}/${CROSS_COMPILER}gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILER_PATH}/${CROSS_COMPILER}g++")
set(CMAKE_LINKER "${CROSS_COMPILER_PATH}/${CROSS_COMPILER}ld")
set(CMAKE_AR "${CROSS_COMPILER_PATH}/${CROSS_COMPILER}ar")

if(NOT IS_ABSOLUTE "${CMAKE_C_COMPILER}")
  message(FATAL_ERROR "FindCompiler provided non-absolute path \"${CMAKE_C_COMPILER}\"!")
endif()

if(NOT EXISTS "${CMAKE_C_COMPILER}")
  message(FATAL_ERROR "FindCompiler provided non-existing path \"${CMAKE_C_COMPILER}\"!")
endif()

if(NOT IS_ABSOLUTE "${CMAKE_CXX_COMPILER}")
  message(FATAL_ERROR "FindCompiler provided non-absolute path \"${CMAKE_CXX_COMPILER}\"!")
endif()

if(NOT EXISTS "${CMAKE_CXX_COMPILER}")
  message(FATAL_ERROR "FindCompiler provided non-existing path \"${CMAKE_CXX_COMPILER}\"!")
endif()

# Configure the behaviour of the find commands
# SET(CMAKE_FIND_ROOT_PATH "")
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message(STATUS "${CMAKE_SYSTEM_NAME}")
message(STATUS "${CMAKE_C_COMPILER}")
message(STATUS "${CMAKE_CXX_COMPILER}")

message(STATUS "see hi3515.cmake: ${CMAKE_CROSSCOMPILING}")
