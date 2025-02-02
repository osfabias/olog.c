cmake_minimum_required(VERSION 3.16)

########### project setup #############
project(olog VERSION 1.0.0 LANGUAGES C)

####### detect standalone build #######
string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" OLOG_STANDALONE)

############# setup cmake #############
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ${OLOG_STANDALONE})

############### options ###############
option(OLOG_BUILD_SHARED "Build olog as a shared library." OFF)
option(OLOG_BUILD_EXAMPLE "Build olog example program." ${OLOG_STANDALONE})

#### set default CMAKE_BUILD_TYPE #####
if(CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "olog: CMAKE_BUILD_TYPE not specified, automatically set to \"Debug\"")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

############# print info ##############
message(STATUS)
message(STATUS "### ${PROJECT_NAME} v${PROJECT_VERSION} ###")
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OLOG_BUILD_SHARED: ${OLOG_BUILD_SHARED}")
message(STATUS "OLOG_BUILD_EXAMPLE: ${OLOG_BUILD_EXAMPLE}")
message(STATUS)

########## target variables ###########
set(OLOG_SOURCE_FILES
  src/olog.c
  # add new source files here...
)

if (MSVC)
  set(OLOG_COMPILE_OPTIONS
    /W4
    # add new compile options here...
  )
else()
  set(OLOG_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic

    -Wno-gnu-zero-variadic-macro-arguments
    # add new compile options here...
  )
endif()

############# add target ##############
if(OLOG_BUILD_SHARED)
  add_library(olog SHARED ${OLOG_SOURCE_FILES})
  target_compile_definitions(olog PRIVATE OLOG_EXPORT)
else()
  add_library(olog STATIC ${OLOG_SOURCE_FILES})
endif()

target_include_directories(olog PUBLIC include)
target_include_directories(olog PRIVATE src)

target_compile_options(olog PRIVATE ${OLOG_COMPILE_OPTIONS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(olog PUBLIC OLOG_DEBUG)
endif()

if(OLOG_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()
