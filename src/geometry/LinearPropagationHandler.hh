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
/**
 * @brief Handler grouping neutral tracks and performing linear propagation.
 */

class LinearPropagationHandler
{
  protected:
  private:
    LinearPropagationHandler(const LinearPropagationHandler&) = delete;
    LinearPropagationHandler& operator=(const LinearPropagationHandler&)
        = delete;

    /** @brief fast move, update only position and step */
    CELER_FORCEINLINE_FUNCTION
    void fast_linear_step(GeoTrackView& track, real_type step);

  public:
    /** @brief Default constructor */
    LinearPropagationHandler() = default;

    /** @briefdestructor */
    ~LinearPropagationHandler() = default;

    /** @brief Scalar DoIt interface */
    CELER_FORCEINLINE_FUNCTION
    void move_next_step(GeoTrackView& track);
};

} // namespace celeritas

#include "geometry/LinearPropagationHandler.i.hh"
