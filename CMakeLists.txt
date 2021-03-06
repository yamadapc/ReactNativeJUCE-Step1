cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if(WIN32)
  # set(WARNING_FLAGS "-Wall")
else()
  # set(WARNING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Werror -pedantic")
endif()

find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy"
)

if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*,-fuchsia-default-arguments")
endif()

project(cpp_javascript_core)

set (
  SOURCES
  "Source/Cpp-JavaScriptCore/Cpp-JavaScriptCore.cpp"
  "Source/Cpp-JavaScriptCore/Cpp-JavaScriptCore.h"
  "Source/Cpp-JavaScriptCore/CJSClass.h"
  "Source/Cpp-JavaScriptCore/CJSConstructor.cpp"
  "Source/Cpp-JavaScriptCore/CJSConstructor.h"
  "Source/Cpp-JavaScriptCore/CJSFunction.cpp"
  "Source/Cpp-JavaScriptCore/CJSFunction.h"
  "Source/Cpp-JavaScriptCore/CJSObject.cpp"
  "Source/Cpp-JavaScriptCore/CJSObject.h"
  "Source/Cpp-JavaScriptCore/CJSUtil.cpp"
  "Source/Cpp-JavaScriptCore/CJSUtil.h"
  "Source/Cpp-JavaScriptCore/CJSValue.cpp"
  "Source/Cpp-JavaScriptCore/CJSValue.h"
  "Source/Cpp-JavaScriptCore/MagicConversion.cpp"
  "Source/Cpp-JavaScriptCore/MagicConversion.h"
)

# Tests executable set-up
add_executable(
  tests

  "${SOURCES}"

  "${CMAKE_SOURCE_DIR}/tests/Main.cpp"
  "${CMAKE_SOURCE_DIR}/tests/AllTests.cpp"
)

target_link_libraries(tests Catch2::Catch2)
target_link_libraries(tests "-framework JavaScriptCore")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/Catch2")

target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/vendor/Catch2/single_include")
target_include_directories(tests SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/external")
target_include_directories(tests SYSTEM PRIVATE "/usr/local/include")

set_target_properties(
  tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

if(CLANG_TIDY_EXE)
  set_target_properties(
    tests PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
  )
endif()
