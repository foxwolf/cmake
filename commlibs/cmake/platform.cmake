#
# If it is a Windows operating system
#   1.Use command line tool of VS2013 or higher version
#     mkdir build; cd build;
#     cmake -G "NMake Makefiles" ..
#     nmake install
#   2.Use the VS development interface tool
#     mkdir build; cd build;
#     cmake -A x64 ..
#     open the file named TDengine.sln
#

# Set macro definitions according to os platform
set(SD_LINUX FALSE)
set(SD_LINUX_32 FALSE)
set(SD_LINUX_64 FALSE)
set(SD_WINDOWS FALSE)
set(SD_WINDOWS_32 FALSE)
set(SD_WINDOWS_64 FALSE)
set(SD_ARM_32 FALSE)
set(SD_ARM_64 FALSE)
set(SD_HI3515 FALSE)
set(SD_HI3531 FALSE)
set(SD_HI3536 FALSE)

# ARM example: cmake .. -DCPU_TYPE=HI3515 or cmake .. -DCPU_TYPE=HI3531
if ("${CPU_TYPE}" MATCHES "HI3515")
  message(STATUS "input cpu_type is: ${CPU_TYPE}")
  set(SD_HI3515 TRUE)
elseif ("${CPUT_TYPE}" MATCHES "HI3531")
  message(STATUS "input cpu_type is: ${CPU_TYPE}")
  set(SD_HI3531 TRUE)
elseif ("${CPUT_TYPE}" MATCHES "HI3536")
  message(STATUS "input cpu_type is: ${CPU_TYPE}")
  set(SD_HI3536 TRUE)  
else ()
  message(STATUS "input cpu_type is: ${CPU_TYPE}")
endif ()

message(STATUS "top_CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "top_CMAKE_SIZEOF_VOID_P: ${CMAKE_SIZEOF_VOID_P}")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
  set(SD_LINUX TRUE)
  if ("${CMAKE_SIZEOF_VOID_P}" MATCHES 8) #64bit
    set(SD_LINUX_64 TRUE)
    MESSAGE(STATUS "The current platform is Linux 64-bit")
  elseif ("${CMAKE_SIZEOF_VOID_P}" MATCHES 4) #32bit
    set(SD_LINUX_32 TRUE)
    MESSAGE(STATUS "The current platform is Linux 32-bit")
  else ()
    message(FATAL_ERROR "The current platform is Linux neither 32-bit nor 64-bit, not supported yet")
  endif ()
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
  set(SD_WINDOWS TRUE)
  if ("${CMAKE_SIZEOF_VOID_P}" MATCHES 8) #64bit
    set(SD_WINDOWS_64 TRUE)
    MESSAGE(STATUS "The current platform is Windows 64-bit")
    elseif ("${CMAKE_SIZEOF_VOID_P}" MATCHES 4) #32bit 
    set(SD_WINDOWS_32 TRUE)
    MESSAGE(STATUS "The current platform is Windows 32-bit")
  else ()
    message(FATAL_ERROR "The current platform is Windows neither 32-bit nor 64-bit, not supported yet")
  endif ()

elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "HI3515")
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "HI3531")
elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "HI3536")
else ()
  message(FATAL_ERROR "The current platfrom is not Linux/Windows, Abort!")

endif ()
