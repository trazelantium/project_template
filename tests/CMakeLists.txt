set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

set(TEST_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_SRC_FILES 
    ${TEST_SRC_PATH}/main.cpp
    ${TEST_SRC_PATH}/print_test.cpp
    )
CheckFileList(TEST_SRC_FILES ${TEST_SRC_PATH})

# Make test executable
add_executable(${TEST_PROJECT_NAME} ${TEST_SRC_FILES})
target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_PATH} ${TEST_THIRD_PARTY_INCLUDE_PATH})
#target_link_libraries(${TEST_PROJECT_NAME} ${PROJECT_NAME})

# Turn on CMake testing capabilities
enable_testing()
ParseAndAddCatchTests (${TEST_PROJECT_NAME})

# coverage settings
if (CMAKE_GNU_COMPILER_ID OR CMAKE_Clang_COMPILER_ID)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")
    find_package(codecov)
    add_coverage(${TEST_PROJECT_NAME})
    # list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
    coverage_evaluate()
endif()
