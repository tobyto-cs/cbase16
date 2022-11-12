cmake_minimum_required(VERSION 3.2)

project(
  cbase16
  VERSION 0.1
  DESCRIPTION "A base16 manager built using c++"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

add_subdirectory(src)
