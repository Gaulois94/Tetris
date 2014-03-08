cmake_minimum_required(VERSION 2.6)
project(tetris)
include_directories(${sdl_include_dir} include)
link_directories(${sdl_lib_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_output_dir})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE "Debug")

set(sdl_name "SDL2" CACHE STRING "Name of the lib")
set(sdl_include_dir "/usr/include" CACHE PATH "Path to the include directory of the lib")
set(sdl_lib_dir "/usr/lib" CACHE PATH "Path to libs")

set(sdl_image_name "SDL2_image" CACHE STRING "Name of sdl image lib")
set(sdl_ttf_name "SDL2_ttf" CACHE STRING "Name of sdl ttf lib")

set(iniparser_name "iniparser" CACHE STRING "Name of the iniparser lib")
set(bin_output_dir "bin/" CACHE PATH "Path to where the program will be build")

file(
	GLOB_RECURSE
	source_file
	src/*
	include/*
)

add_executable(
	tetris
	${source_file}
)

target_link_libraries(
	tetris
	${sdl_name}
	${sdl_image_name}
	${sdl_ttf_name}
	${iniparser_name}
	m
)	
