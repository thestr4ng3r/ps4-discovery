cmake_minimum_required(VERSION 3.7)
project(ps4_discovery)

set(SOURCE_FILES
		src/main.c
		src/common.c
		src/common.h
		src/discover.c
		src/discover.h
		src/spoof.c
		src/spoof.h)

add_executable(ps4_discovery ${SOURCE_FILES})