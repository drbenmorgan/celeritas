#----------------------------------*-CMake-*----------------------------------#
# Copyright 2023 UT-Battelle, LLC, and other Celeritas developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.18)
project(CeleritasCorecelExample VERSION 0.0.1 LANGUAGES CXX)
cmake_policy(VERSION 3.12...3.22)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")
find_package(Celeritas REQUIRED)

add_executable(collection collection.cc)
target_link_libraries(collection PRIVATE Celeritas::corecel)

if(CELERITAS_USE_CUDA OR CELERITAS_USE_HIP)
  add_executable(saxpy_devicevector saxpy_devicevector.cc saxpy.hh saxpy.cu)
  target_link_libraries(saxpy_devicevector PRIVATE Celeritas::corecel)
endif()
