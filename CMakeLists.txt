cmake_minimum_required(VERSION 3.15)

project(json-maker
        VERSION 1.1.0.0
        DESCRIPTION "JSON Maker is a C library used to code JSON objects in null-terminated strings."
        HOMEPAGE_URL "https://github.com/rafagafe/json-maker/blob/master/makefile"
        LANGUAGES C
        )

set(CMAKE_COLOR_MAKEFILE  ON CACHE BOOL "ON" FORCE)
set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT)

add_compile_options(-std=c99 -Wall -pedantic)

add_subdirectory(src)