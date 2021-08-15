add_library(base INTERFACE)
target_include_directories(base INTERFACE include)

add_subdirectory(unit-tests)
