AUX_SOURCE_DIRECTORY(. SRC)

ADD_LIBRARY(os ${SRC})
include_directories(.)

TARGET_LINK_LIBRARIES(os)
