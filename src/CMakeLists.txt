add_library(json_maker_api INTERFACE)
target_include_directories(json_maker_api INTERFACE include)

add_library(json_maker STATIC)
target_sources(json_maker PUBLIC json-maker.c)
target_link_libraries(json_maker PUBLIC json_maker_api)
