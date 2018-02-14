cmake_minimum_required(VERSION 3.3)

# project settings
project(project_template VERSION 1.0.0)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

#checks that the given hard-coded list contains all headers + sources in the given folder
function(CheckFileList LIST_VAR FOLDER)
  set(MESSAGE " should be added to the variable ${LIST_VAR}")
  set(MESSAGE "${MESSAGE} in ${CMAKE_CURRENT_LIST_FILE}\n")
  file(GLOB GLOBBED_LIST "${FOLDER}/*.cpp"
                         "${FOLDER}/*.hpp"
                         "${FOLDER}/*.h")
  list(REMOVE_ITEM GLOBBED_LIST ${${LIST_VAR}})
  foreach(EXTRA_ITEM ${GLOBBED_LIST})
    string(REPLACE "${CATCH_DIR}/" "" RELATIVE_FILE_NAME "${EXTRA_ITEM}")
    message(AUTHOR_WARNING "The file \"${RELATIVE_FILE_NAME}\"${MESSAGE}")
  endforeach()
endfunction()

function(CheckFileListRec LIST_VAR FOLDER)
  set(MESSAGE " should be added to the variable ${LIST_VAR}")
  set(MESSAGE "${MESSAGE} in ${CMAKE_CURRENT_LIST_FILE}\n")
  file(GLOB_RECURSE GLOBBED_LIST "${FOLDER}/*.cpp"
                                 "${FOLDER}/*.hpp"
                                 "${FOLDER}/*.h")
  list(REMOVE_ITEM GLOBBED_LIST ${${LIST_VAR}})
  foreach(EXTRA_ITEM ${GLOBBED_LIST})
    string(REPLACE "${CATCH_DIR}/" "" RELATIVE_FILE_NAME "${EXTRA_ITEM}")
    message(AUTHOR_WARNING "The file \"${RELATIVE_FILE_NAME}\"${MESSAGE}")
  endforeach()
endfunction()


# specify project files
set(PROJECT_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_HEADER_FILES
    ${PROJECT_INCLUDE_PATH}/print.hpp
    )
CheckFileList(PROJECT_HEADER_FILES ${PROJECT_INCLUDE_PATH})

set(PROJECT_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_SRC_FILES
    ${PROJECT_SRC_PATH}/main.cpp
    )
CheckFileList(PROJECT_SRC_FILES ${PROJECT_SRC_PATH})

# # header only library
# add_library(${PROJECT_NAME} INTERFACE)
# target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH}) 

# library
# add_library(${PROJECT_NAME} ${PROJECT_SRC_FILES})

# executable
add_executable(${PROJECT_NAME} ${PROJECT_SRC_FILES})

add_subdirectory (third_party EXCLUDE_FROM_ALL)

# include
target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_INCLUDE_PATH}>
    $<BUILD_INTERFACE:${THIRD_PARTY_INCLUDE_PATH}>
    $<INSTALL_INTERFACE:include>
    PRIVATE ${PROJECT_SRC_PATH})

# dependencies
set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)


# compile settings
set (CMAKE_CXX_STANDARD 17)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (CMAKE_AppleClang_COMPILER_ID AND POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()


if (CMAKE_MSVC_COMPILER_ID)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /premissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


# test and coverage config
option(ENABLE_TESTS_AND_COVERAGE "Enable to build tests and coverage reporting." ON)
if (ENABLE_TESTS_AND_COVERAGE)
    enable_testing()
    add_subdirectory(tests)
endif()

# documentation

# install libraries into correct locations on all platforms.
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}Config"
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${PROJECT_INCLUDE_PATH} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT "${PROJECT_NAME}Config" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" 
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# This makes the project importable from the build directory
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

