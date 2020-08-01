//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file Action.hh
//---------------------------------------------------------------------------//
#pragma once

#include "base/Macros.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Broad categories of events applied to a particle in a track.
 */
enum class Action
{
    failure, //!< Recoverable error in sampling
    birth,   //!< Primary particle from an event or creation of a secondary
    cell,    //!< Enter a new cell
    scatter, //!< Undergo a scattering event
    // KILLING ACTIONS BELOW
    begin_kill_,
    absorb = begin_kill_, //!< Absorbed (killed)
    energy_cutoff,        //!< Below energy cutoff (killed)
    escape,               //!< Exited geometry (killed)
    end_kill_
};

//---------------------------------------------------------------------------//
// INLINE FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * Whether the function returning an action succeeded.
 *
 * This should be 'false' for (e.g.) failing to allocate memory for sampling
 * secondaries, allowing a recoverable failure (next kernel launch retries with
 */
inline CELER_FUNCTION bool action_completed(Action a)
{
    return a != Action::failure;
}

//---------------------------------------------------------------------------//
/*!
 * Whether the given action should kill the active track.
 */
inline CELER_FUNCTION bool action_kills(Action a)
{
    return int(a) >= int(Action::begin_kill_)
           && int(a) < int(Action::end_kill_);
}

//---------------------------------------------------------------------------//
} // namespace celeritas
