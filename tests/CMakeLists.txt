
macro(add_testx test_name)
    add_executable(${test_name} ${test_name}.cpp)
    target_include_directories(
        ${test_name} SYSTEM PRIVATE
        ${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        )
    target_link_libraries(${test_name} gtest gmock gtest_main)
endmacro()
enable_testing()
add_test(dummy dummy)
