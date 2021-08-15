
add_unit_test(main main.cpp)
target_link_libraries(main PRIVATE base)

if (MYOWNTBB)
  target_include_directories(main PUBLIC /home/antharion/Documents/repos/tbb/include)
  target_link_libraries(main PUBLIC /home/antharion/Documents/repos/tbb/build/linux_intel64_gcc_cc7.5.0_libc2.27_kernel4.15.0_release/libtbb.so.2)
else ()
  #find_package(tbb)
  target_link_libraries(main PUBLIC tbb)
endif()
