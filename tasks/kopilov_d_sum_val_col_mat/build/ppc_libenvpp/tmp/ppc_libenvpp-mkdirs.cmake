# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/PPC/ppc-2025-processes-engineers/3rdparty/libenvpp")
  file(MAKE_DIRECTORY "D:/PPC/ppc-2025-processes-engineers/3rdparty/libenvpp")
endif()
file(MAKE_DIRECTORY
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/build"
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/install"
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/tmp"
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/src/ppc_libenvpp-stamp"
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/src"
  "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/src/ppc_libenvpp-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/src/ppc_libenvpp-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/PPC/ppc-2025-processes-engineers/tasks/kopilov_d_sum_val_col_mat/build/ppc_libenvpp/src/ppc_libenvpp-stamp${cfgdir}") # cfgdir has leading slash
endif()
