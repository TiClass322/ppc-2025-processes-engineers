# CMake generated Testfile for 
# Source directory: D:/PPC/ppc-2025-processes-engineers/modules
# Build directory: D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/modules
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[core_func_tests]=] "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/bin/core_func_tests.exe")
  set_tests_properties([=[core_func_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;48;add_test;D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[core_func_tests]=] "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/bin/core_func_tests.exe")
  set_tests_properties([=[core_func_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;48;add_test;D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[core_func_tests]=] "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/bin/MinSizeRel/core_func_tests.exe")
  set_tests_properties([=[core_func_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;48;add_test;D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[core_func_tests]=] "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/bin/RelWithDebInfo/core_func_tests.exe")
  set_tests_properties([=[core_func_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;48;add_test;D:/PPC/ppc-2025-processes-engineers/modules/CMakeLists.txt;0;")
else()
  add_test([=[core_func_tests]=] NOT_AVAILABLE)
endif()
