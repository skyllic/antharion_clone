cmake_minimum_required(VERSION 3.7)

project(ExponentialIdleTheories)

option(MYOWNTBB "Only Antharion should use this" False)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set (CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

function(add_unit_test executable source_file)
  add_executable(${executable} ${source_file})
  target_include_directories(${executable} PUBLIC ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${executable} PUBLIC ${GTEST_LIBRARIES} Threads::Threads)

  add_test(
      NAME ${executable}
      COMMAND ${executable}
  )
endfunction()
enable_testing()
add_subdirectory(components)
add_subdirectory(executables)
