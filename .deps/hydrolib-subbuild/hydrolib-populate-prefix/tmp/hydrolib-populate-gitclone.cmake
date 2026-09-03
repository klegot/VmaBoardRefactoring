# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitclone-lastrun.txt" AND EXISTS "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitinfo.txt" AND
  "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/workspaces/VmaBoardRefactoring/.deps/hydrolib-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/workspaces/VmaBoardRefactoring/.deps/hydrolib-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/local/bin/git" 
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/SeaJackal/Hydrolib-soft.git" "hydrolib-src"
    WORKING_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/SeaJackal/Hydrolib-soft.git'")
endif()

execute_process(
  COMMAND "/usr/local/bin/git" 
          checkout "1c21669707ba36e2cd95d37db00bafc62fdc1d51" --
  WORKING_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/hydrolib-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '1c21669707ba36e2cd95d37db00bafc62fdc1d51'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/local/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/workspaces/VmaBoardRefactoring/.deps/hydrolib-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/workspaces/VmaBoardRefactoring/.deps/hydrolib-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitinfo.txt" "/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/workspaces/VmaBoardRefactoring/.deps/hydrolib-subbuild/hydrolib-populate-prefix/src/hydrolib-populate-stamp/hydrolib-populate-gitclone-lastrun.txt'")
endif()
