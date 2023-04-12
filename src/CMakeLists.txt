cmake_minimum_required(VERSION 3.2)

# set(HEADER_FILES ./utils/po_struct.hpp ./actions/build.hpp)

add_executable(cbase16 main.cpp)

target_sources(
  cbase16
  PRIVATE actions/build.cpp
          utils/po_struct.cpp
          scheme/scheme.cpp
          utils/updater.cpp
          actions/inject.cpp
          actions/list.cpp
          utils/file_helpers.cpp)

target_include_directories(cbase16 PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(Boost_FOUND)
  message(STATUS "Found boost: ${Boost_LIBRARIES}")
  target_link_libraries(cbase16 PRIVATE ${Boost_LIBRARIES})
endif()

if(LIBGIT2_FOUND)
  message(STATUS "Found libgit2: ${LIBGIT2_LIBRARIES}")
  target_link_libraries(cbase16 PUBLIC ${LIBGIT2_LIBRARIES})
endif()

target_link_libraries(cbase16 PRIVATE fmt::fmt-header-only)
target_link_libraries(cbase16 PUBLIC ryml::ryml)
# target_link_libraries(cbase16 PUBLIC $<BUILD_INTERFACE:ryml::ryml>
# $<INSTALL_INTERFACE:include/ryml>)
target_link_libraries(cbase16 PUBLIC mstch)
target_link_libraries(cbase16 PUBLIC crypto)

target_link_libraries(cbase16 PUBLIC curl)
