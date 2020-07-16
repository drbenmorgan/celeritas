#----------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

if(CELERITAS_USE_GEANT4)
  # TODO: switch to target-based link/include once Geant4 supports it
  include(${Geant4_USE_FILE})

  add_executable(geant-exporter geant-export/geant-export.cc)
  target_link_libraries(geant-exporter celeritas)
endif()

#-----------------------------------------------------------------------------#
