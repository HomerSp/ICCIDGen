cmake_minimum_required(VERSION 3.1.0)

SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc-win32)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++-win32)
SET(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(PROJECT "iccidgen")
project(${PROJECT})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

list(APPEND LIB_LIST -lws2_32 -static)

include_directories(
	include
)

set(HEADERS
	include/bitstream.h
	include/encrypt.h
	include/hash.h
	include/utils.h
)

set(SOURCES
	main.cpp
	encrypt.cpp
	hash.cpp
	utils.cpp
	bitstream.cpp
)

source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT}
	${LIB_LIST}
)
