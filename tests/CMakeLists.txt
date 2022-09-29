include_directories (${CMAKE_SOURCE_DIR}/src)

file(GLOB TEST_EXECUTABLES CONFIGURE_DEPENDS
	"*.cpp")

message("${TEST_EXECUTABLES}")

foreach(TEST ${TEST_EXECUTABLES})
  get_filename_component(TEST_NAME ${TEST} NAME_WLE)
  message(STATUS "Creating test ${TEST_NAME} for ${TEST}")
  add_executable(${TEST_NAME} ${TEST})
  target_link_libraries(${TEST_NAME} cbase)
  add_test(${TEST_NAME}, ${TEST_NAME})
endforeach()

# add_executable (testing test_test.cpp)
# target_link_libraries (testing cbase)

# add_test(test1 testing)
