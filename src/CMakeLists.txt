add_library(json_maker STATIC)

target_sources(json_maker PUBLIC json-maker.c)
target_include_directories(json_maker PUBLIC include)
