//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTable.hh
//---------------------------------------------------------------------------//
#pragma once

#include <vector>

#include "GeantPhysicsVector.hh"
#include "base/Types.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Struct to store Geant4 asc physics tables.
 */

/*
struct GeantPhysicsTable
{
    ssize_type                          tableSize_;
    std::vector<real_type>              edgeMin_;
    std::vector<real_type>              edgeMax_;
    std::vector<ssize_type>             numberOfNodes_;
    std::vector<ssize_type>             vectorType_;
    std::vector<std::vector<real_type>> binVector_;
    std::vector<std::vector<real_type>> dataVector_;
};
*/

struct GeantPhysicsTable
{
    std::vector<GeantPhysicsVector> physics_vectors;
};

//---------------------------------------------------------------------------//
} // namespace celeritas
