if (NOT DEFINED ENV{IDF_PATH})
	cmake_minimum_required(VERSION 3.5.2)
	
else()

set(srcs "src/sd_util.c"
		 "src/sd_msgsink.c"
		 "src/kfifo.c"
		 "src/sd_listlib.c"
		 "src/sd_log.c"
		 "src/sd_global.c"
		 "src/sd_ntp_check.c"
		 "src/sd_ntp_client.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    REQUIRES os modbus)
endif()