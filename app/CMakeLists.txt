#----------------------------------*-CMake-*----------------------------------#
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
#-----------------------------------------------------------------------------#

if(CELERITAS_USE_GEANT4)
  # TODO: switch to target-based link/include once Geant4 supports it
  #include(${Geant4_USE_FILE})

  add_executable(geant-exporter geant-exporter/geant-exporter.cc
    geant-exporter/src/ActionInitialization.cc
    geant-exporter/src/DetectorConstruction.cc
    geant-exporter/src/EventAction.cc
    geant-exporter/src/GeantPhysicsTableParser.cc
    geant-exporter/src/PrimaryGeneratorAction.cc
    geant-exporter/src/RunAction.cc
    geant-exporter/src/SteppingAction.cc
    )

  target_include_directories(geant-exporter PRIVATE
    ${Geant4_INCLUDE_DIR}
    )


  target_link_libraries(geant-exporter
    celeritas
    ROOT::Core ROOT::Tree
    XercesC::XercesC
    ${Geant4_LIBRARIES}
    )

endif()

#-----------------------------------------------------------------------------#
