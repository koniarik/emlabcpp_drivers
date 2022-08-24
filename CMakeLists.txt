cmake_minimum_required(VERSION 3.16)

option(EMLABCPP_DRIVERS_TESTS_ENABLED "Decides whenever tests should be enabled" OFF)

project(emlabcpp_drivers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
	emlabcpp_drivers
	STATIC
	src/emlabcpp/drivers/INA219/driver.cpp
	)

include(../emlabcpp/cmake/util.cmake)

target_include_directories(emlabcpp_drivers PUBLIC src/)
emlabcpp_compile_options(emlabcpp_drivers)
target_link_libraries(emlabcpp_drivers PRIVATE emlabcpp)

if(EMLABCPP_DRIVERS_TESTS_ENABLED)
  add_subdirectory(../emlabcpp ${CMAKE_CURRENT_BINARY_DIR}/emlabcpp)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()
endif()
