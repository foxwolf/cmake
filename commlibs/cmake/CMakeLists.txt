
if (NOT DEFINED ENV{IDF_PATH})
  cmake_minimum_required(VERSION 3.5.2)
  project(commlibs)

#set(CMAKE_BUILD_TYPE "Debug")

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_COLOR_MAKEFILE ON)

#PROJECT_SOURCE_DIR 和project命令是相关的
  set(SD_DIR ${PROJECT_SOURCE_DIR}/../)

  message(STATUS "Operating system dependency directory: " ${SD_DIR})
  message(STATUS "Project source directory: " ${PROJECT_SOURCE_DIR})
  message(STATUS "Project binary files output path: " ${PROJECT_BINARY_DIR})
  message(STATUS "Project executable files output path: " ${EXECUTABLE_OUTPUT_PATH})
  message(STATUS "Project library files output path: " ${LIBRARY_OUTPUT_PATH})
  message(STATUS "The Current platform " ${CMAKE_SYSTEM_PROCESSOR} " is detected")

  include(platform.cmake)
  
  include(define.cmake)
  include(install.cmake)
  include(env.cmake)

  add_subdirectory(../os os.out)
  add_subdirectory(../util util.out)
else ()

endif()



