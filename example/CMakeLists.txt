add_executable(example main.c)
target_link_libraries(example PRIVATE olog)
target_compile_options(example PRIVATE ${OLOG_COMPILE_OPTIONS})
