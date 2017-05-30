cmake_minimum_required(VERSION 3.7)
project(ps4_discovery)

set(SOURCE_FILES
		src/main.c
		src/common.c
		src/common.h
		src/discover.c
		src/discover.h
		src/spoof.c
		src/spoof.h
		src/cmd_spoof.c
		src/cmd_spoof.h
		src/argp_utils.c
		src/argp_utils.h
		src/cmd_discover.c
		src/cmd_discover.h)

add_executable(ps4_discovery ${SOURCE_FILES})