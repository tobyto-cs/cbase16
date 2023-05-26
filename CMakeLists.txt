cmake_minimum_required(VERSION 3.2)

project(
  cbase16
  VERSION 0.1
  DESCRIPTION "A base16 manager built using c++"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std=c++2a")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wextra -Wpedantic -Wuninitialized -Wmissing-include-dirs\
  -Wundef -Wstrict-aliasing -fsanitize=address -fno-omit-frame-pointer")
message("Compiler flags: ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wno-dev")

# LeakSanitizer Suppression settings for external libraries.
set(ENV{LSAN_OPTIONS} "suppressions=${CMAKE_SOURCE_DIR}/dev/lsan.supp")

find_package(
  Boost 1.80.0
  COMPONENTS program_options
  REQUIRED)
if(Boost_FOUND)
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_MULTITRHEADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# DYNAMIC LIBS
find_package(libgit2 REQUIRED)
if(LIBGIT2_FOUND)
  include_directories(${LIBGIT2_INCLUDE_DIR})
endif()

find_package(CURL REQUIRED)

# find_package(mstch)

# STATIC LIBS EXCLUDE_FROM_ALL is required so these libs aren't install()
add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
add_subdirectory(external/rapidyaml EXCLUDE_FROM_ALL)
add_subdirectory(external/mstch EXCLUDE_FROM_ALL)
include_directories(${mstch_INCLUDE_DIR})

add_subdirectory(src)
