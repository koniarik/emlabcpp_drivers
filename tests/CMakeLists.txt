include(GoogleTest)

find_package(GTest REQUIRED)

# duplication from emlabcpp tests
target_compile_options(
  emlabcpp
  PUBLIC -DEMLABCPP_USE_NLOHMANN_JSON -DEMLABCPP_ASSERT_NATIVE
         -DEMLABCPP_USE_MAGIC_ENUM -DEMLABCPP_USE_GTEST -DEMLABCPP_USE_LOGGING
         -DEMLABCPP_USE_DEMANGLING)
target_include_directories(
  emlabcpp SYSTEM PUBLIC ${EMLABCPP_DRIVERS_EMLABCPP_SRC}/tests/third-party/)

function(add_emlabcpp_driver_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} GTest::GTest GTest::Main)

  target_link_libraries(${name} emlabcpp emlabcpp_drivers_${name})
  add_test(NAME ${name}_test COMMAND ${name})
endfunction()

add_emlabcpp_driver_test(INA219)
