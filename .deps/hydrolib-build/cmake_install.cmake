# Install script for directory: /workspaces/VmaBoardRefactoring/.deps/hydrolib-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/local/bin/arm-none-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_bus/hydrolib_bus_application/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_bus/hydrolib_bus_datalink/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_concepts/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_crc/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_device/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_filter/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_imu/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_logger/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_math/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_pid/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_pressure_sensor/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_return_codes/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_shell_commands/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_shell/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_streams/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_strings/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_thrust_generator/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_vectornav/cmake_install.cmake")
  include("/workspaces/VmaBoardRefactoring/.deps/hydrolib-build/hydrolib_ring_queue/cmake_install.cmake")

endif()

