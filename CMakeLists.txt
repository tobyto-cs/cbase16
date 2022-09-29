cmake_minimum_required(VERSION 3.5)
project(cbase16 CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Add lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS program_options)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/rapidyaml)
set(RYML_STANDALONE ON)

configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
