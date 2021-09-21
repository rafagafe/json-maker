add_executable(json_maker_test test.c)
target_link_libraries(json_maker_test PRIVATE json_maker)

add_test(run_main_tests json_maker_test)