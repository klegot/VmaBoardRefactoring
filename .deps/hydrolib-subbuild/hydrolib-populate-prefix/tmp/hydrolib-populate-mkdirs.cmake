# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-src"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-build"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/tmp"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src"
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
