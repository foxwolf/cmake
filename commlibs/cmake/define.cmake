
cmake_minimum_required(VERSION 3.5.2)

if (SD_LINUX_32)
	add_definitions(-D_SD_LINUX_32)
endif ()

if (SD_LINUX_64)
	add_definitions(-D_SD_LINUX_64)
endif ()

if (SD_WINDOWS_32)
	add_definitions(-D_SD_WINDOWS_32)
endif ()

if (SD_WINDOWS_64)
	add_definitions(-D_SD_WINDOWS_64)
endif ()

include_directories(${SD_DIR}/os/include)
include_directories(${SD_DIR}/deps/pthread)

