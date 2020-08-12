//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantTableType.hh
//---------------------------------------------------------------------------//
#pragma once

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * /brief Enumerator for the physics table types
 *
 * This enum does not exists in Geant4. It was created to safely access
 * the many tables imported from it.
 */
enum class GeantTableType
{
    not_defined,
    DEDX,
    Ionisation,
    Range,
    RangeSec,
    InverseRange,
    Lambda,
    LambdaPrim,
    LambdaMod1,
    LambdaMod2,
    LambdaMod3,
    LambdaMod4
};

//---------------------------------------------------------------------------//
} // namespace celeritas