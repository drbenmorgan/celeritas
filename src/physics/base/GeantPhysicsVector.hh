//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsVector.hh
//---------------------------------------------------------------------------//
#pragma once

#include "GeantPhysicsVectorType.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Struct to store Geant4 physics table data imported from the ROOT file
 * produced by the geant-exporter external app.
 */
struct GeantPhysicsVector
{
    GeantPhysicsVectorType vector_type;
    std::vector<real_type> energy;   // Geant4 binVector
    std::vector<real_type> xs_eloss; // Geant4 dataVector
};

//---------------------------------------------------------------------------//
} // namespace celeritas

