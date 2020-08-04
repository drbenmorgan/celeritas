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
 * Geant4 PhysicsVector type enums
 */
enum GeantPhysicsVectorType
{
    Geant4PhysicsVector = 0,
    Geant4PhysicsLinearVector,
    Geant4PhysicsLogVector,
    Geant4PhysicsLnVector,
    Geant4PhysicsFreeVector,
    Geant4PhysicsOrderedFreeVector,
    Geant4LPhysicsFreeVector
};

//---------------------------------------------------------------------------//
} // namespace celeritas

