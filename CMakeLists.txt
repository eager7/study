cmake_minimum_required(VERSION 3.5)
project(test)
SET(TARGET test)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -m32")
include_directories(
		include 
        )
include_directories(/usr/include/sys)
link_directories(./lib)

set(SOURCE_FILES  
		main.c
        )

#add_library(libsrp.a STATIC)

add_executable(${TARGET} ${SOURCE_FILES} )
#target_link_libraries(${TARGET} -lpthread)
