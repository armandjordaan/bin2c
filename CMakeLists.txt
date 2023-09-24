cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
PROJECT (bin2c)

set( NAME_SRC
    src/argparse.c
    src/bin2c.c
    src/config.c
    src/fileutils.c
    src/get_substr.c
    src/jsmn.c
    src/jsmnhelper.c
    src/main.c
    src/stringhelpers.c
)

set( NAME_HEADERS
    src/argparse.h
    src/bin2c.h
    src/config.h
    src/fileutils.h
    src/get_substr.h
    src/jsmn.h
    src/jsmnhelper.h
    src/main.h
    src/seethe.h
    src/stringhelpers.h
)

configure_file(data/default.json ${CMAKE_CURRENT_BINARY_DIR}/default.json COPYONLY)
configure_file(data/test.bin ${CMAKE_CURRENT_BINARY_DIR}/test.bin COPYONLY)

add_compile_options(
    "-Wall" "-Wextra" "-Wno-missing-field-initializers"
    "$<$<CONFIG:DEBUG>:-O0;-ggdb3;-DDEBUG_BUILD;-DSEETHE_LOGLEVEL=DEBUG>"
    "$<$<CONFIG:RELEASE>:-Os;-Werror;-DRELEASE_BUILD>"
)

add_executable(bin2c ${NAME_SRC} ${NAME_HEADERS} )

target_link_libraries(bin2c)
