cmake_minimum_required(VERSION 3.29)
project(
  collectc
  VERSION 0.1.0
  LANGUAGES C)

set(C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_SKIP_TEST_ALL_DEPENDENCY OFF)

add_compile_options(-Wall -Wpedantic)
enable_testing()

find_package(Doxygen)
if(Doxygen_FOUND)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set(DOXYGEN_STRIP_FROM_PATH include)
  set(DOXYGEN_SHOW_USED_FILES NO)
  doxygen_add_docs(doc README.md include)
endif()

set(PROJECT_TEST_SOURCE_FILES test/main.c test/test.c test/vector.c)

add_library(${PROJECT_NAME} src/vector.c)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(${PROJECT_NAME}-test ${PROJECT_TEST_SOURCE_FILES})
target_include_directories(${PROJECT_NAME}-test PRIVATE include)
target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})

add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)
