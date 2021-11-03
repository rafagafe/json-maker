add_executable(json_maker_test test.c)
target_link_libraries(json_maker_test PRIVATE json_maker)

add_test(NAME run_main_tests COMMAND json_maker_test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
