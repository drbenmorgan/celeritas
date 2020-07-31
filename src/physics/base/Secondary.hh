//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Secondary.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/Types.hh"
#include "ParticleDef.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * New particle created via an Interaction.
 *
 * It will be converted into a track using the original track information.
 */
struct Secondary
{
    ParticleDefId def_id;    //!< New particle type
    real_type     energy;    //!< New kinetic energy
    Real3         direction; //!< New direction

    // Secondary failed to sample
    static inline CELER_FUNCTION Secondary from_failure();

    // Whether the secondary was successfully sampled
    explicit inline CELER_FUNCTION operator bool() const;
};

//---------------------------------------------------------------------------//
// INLINE FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * Construct a secondary that failed to sample correctly.
 */
CELER_FUNCTION Secondary Secondary::from_failure()
{
    Secondary result;
    result.def_id = {};
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * Whether the Secondary succeeded
 */
CELER_FUNCTION Secondary::operator bool() const
{
    return static_cast<bool>(this->def_id);
}

//---------------------------------------------------------------------------//
} // namespace celeritas
