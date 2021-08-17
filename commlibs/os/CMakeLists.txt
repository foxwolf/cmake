
if (DEFINED ENV{IDF_PATH})
	set(srcs "src/freertos/esp32/freertos_init.c"
		"src/freertos/esp32/freertos_socket.c")

	list(APPEND srcs
		"src/freertos/freertos_posix_semaphore.c"
    "src/freertos/freertos_platform.c")

	list(APPEND srcs
		"src/porting/os_socket.c"
		"src/porting/os_misc.c"
		"src/porting/os_file.c")

	idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES net util spiffs)
else ()
  if (SD_LINUX)
    add_subdirectory(src/linux)
  endif()
endif ()


