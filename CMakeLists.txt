cmake_minimum_required(VERSION 3.16)

option(EMLABCPP_DRIVERS_TESTS_ENABLED
       "Decides whenever tests should be enabled" OFF)
option(EMLABCPP_DRIVERS_EMLABCPP_SRC "Gives a location of emlabcpp source")

project(emlabcpp_drivers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(EMLABCPP_DRIVERS_EMLABCPP_SRC)
  if(NOT IS_ABSOLUTE ${EMLABCPP_DRIVERS_EMLABCPP_SRC})
    set(EMLABCPP_DRIVERS_EMLABCPP_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/${EMLABCPP_DRIVERS_EMLABCPP_SRC}")
  endif()
  add_subdirectory(${EMLABCPP_DRIVERS_EMLABCPP_SRC} emlabcpp)
  include(${EMLABCPP_DRIVERS_EMLABCPP_SRC}/cmake/util.cmake)
endif()

add_library(emlabcpp_drivers_base INTERFACE)
target_include_directories(emlabcpp_drivers_base INTERFACE src/)
target_link_libraries(emlabcpp_drivers_base INTERFACE emlabcpp)

add_library(emlabcpp_drivers_INA219 STATIC
            src/emlabcpp/drivers/INA219/driver.cpp)
target_link_libraries(emlabcpp_drivers_INA219 PUBLIC emlabcpp_drivers_base)
emlabcpp_compile_options(emlabcpp_drivers_INA219)

if(EMLABCPP_DRIVERS_TESTS_ENABLED)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()
endif()
