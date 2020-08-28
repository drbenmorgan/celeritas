//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Types.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/OpaqueId.hh"
#include "base/Types.hh"

namespace celeritas
{
class Geometry;
//---------------------------------------------------------------------------//
using VolumeId = OpaqueId<Geometry, unsigned int>;

// a small extra push to ensure that track goes beyond boundaries
constexpr static real_type kgeomTolerance = 1.0e-12; // cm (default length
                                                     // unit)

//---------------------------------------------------------------------------//
} // namespace celeritas
