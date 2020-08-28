//---------------------------------*-CUDA-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file LinearPropagationHandler.hh
//---------------------------------------------------------------------------//
#pragma once

#include "geometry/GeoTrackView.hh"
#include "base/NumericLimits.hh"

namespace celeritas
{
/*!
 * \brief Handler applying linear propagation to neutral tracks.
 */
class LinearPropagationHandler
{
  public:
    // Construct from persistent and state data
    CELER_FUNCTION
    LinearPropagationHandler(GeoTrackView& track) : track_(track) {}

    /*!
     * \brief Move track across a step.
     */
    CELER_FORCEINLINE_FUNCTION
    void move_next_step();

  private:
    /*!
     * \brief Fast move, update only the position.
     */
    CELER_FORCEINLINE_FUNCTION
    void fast_linear_step(real_type step);

  private:
    //@{
    //! Referenced thread-local data
    GeoTrackView& track_;
    //@}
};

} // namespace celeritas

#include "geometry/LinearPropagationHandler.i.hh"
