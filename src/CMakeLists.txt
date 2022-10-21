# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

# Remove main.cpp from library definition
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)


add_library(cbase ${SOURCE_FILES} ${HEADER_FILES})
set_property(TARGET cbase PROPERTY CXX_STANDARD 17)
set_property(TARGET cbase PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(cbase ryml::ryml)
target_link_libraries(cbase fmt::fmt)

include_directories(${mstch_INCLUDE_DIR})
target_link_libraries(cbase mstch)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(cbase ${Boost_LIBRARIES})
endif()

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} cbase)
