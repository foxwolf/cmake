
if (SD_LINUX)

elseif (SD_WINDOWS)

else ()

endif ()

#set output directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/bin)
set(TD_TESTS_OUTPUT_DIR ${PROJECT_BINARY_DIR}/test)

