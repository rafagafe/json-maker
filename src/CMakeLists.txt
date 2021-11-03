add_library(json_maker_api INTERFACE)
target_include_directories(json_maker_api INTERFACE include)
set_target_properties(json_maker_api PROPERTIES PUBLIC_HEADER include/json-maker/json-maker.h)

add_library(json_maker STATIC)
target_sources(json_maker PUBLIC json-maker.c)
target_link_libraries(json_maker PUBLIC json_maker_api)

include(GNUInstallDirs)
install(TARGETS json_maker json_maker_api
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/json_maker
        )
