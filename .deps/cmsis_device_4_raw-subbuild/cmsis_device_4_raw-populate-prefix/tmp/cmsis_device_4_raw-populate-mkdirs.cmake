# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-src"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-build"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/tmp"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/src/cmsis_device_4_raw-populate-stamp"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/src"
  "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/src/cmsis_device_4_raw-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/src/cmsis_device_4_raw-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/cmsis_device_4_raw-subbuild/cmsis_device_4_raw-populate-prefix/src/cmsis_device_4_raw-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
