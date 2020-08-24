//---------------------------------*-CUDA-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file LinearPropagation.i.hh
//---------------------------------------------------------------------------//
#pragma once

#include "geometry/LinearPropagationHandler.hh"
#include "base/ArrayUtils.hh"
#include "geometry/Types.hh"

namespace celeritas
{
//______________________________________________________________________________
// Do straight propagation to physics process or boundary
// Return false if problems (track stuck at boundary after 10 steps), or true
// otherwise.
//
CELER_FORCEINLINE_FUNCTION
void LinearPropagationHandler::fast_linear_step(GeoTrackView& track,
                                                real_type     step)
{
    axpy(step, track.dir(), &track.pos());
}

CELER_FORCEINLINE_FUNCTION
void LinearPropagationHandler::move_next_step(GeoTrackView& track)
{
    // Scalar geometry length computation. The track is moved along track.dir()
    // direction by a distance track.next_step()
    fast_linear_step(track, track.next_step() + 1.0e-12);

    // ensure this is called at least once to update _vgstate
    track.has_same_path();
}

} // namespace celeritas
