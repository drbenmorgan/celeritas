//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsVectorType.hh
//---------------------------------------------------------------------------//
#pragma once

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Geant4 PhysicsVector type enumerator.
 * (See Geant4's G4PhysicsVectorType.hh)
 */
enum GeantPhysicsVectorType
{
    physics_vector = 0,
    physics_linear_vector,
    physics_log_vector,
    physics_ln_vector,
    physics_free_vector,
    physics_ordered_free_vector,
    lphysics_free_vector
};

//---------------------------------------------------------------------------//
} // namespace celeritas
