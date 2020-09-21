//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsArrayCalculator.i.hh
//---------------------------------------------------------------------------//
#include <cmath>
#include "base/Interpolator.hh"
#include "base/UniformGridView.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults.
 */
CELER_FUNCTION real_type
PhysicsArrayCalculator::operator()(const ParticleTrackView& particle) const
{
    UniformGridView loge_grid(data_.log_energy);
    real_type       loge = std::log(particle.energy());

    // Snap out-of-bounds values to closest grid points
    if (loge <= loge_grid.front())
        return data_.xs.front();
    else if (loge >= loge_grid.back())
        return data_.xs.back();

    // Get the energy bin
    auto bin = loge_grid.find(loge);
    CHECK(bin + 1 < data_.xs.size());

    // Interpolate using the bin data.
    // NOTE that this is linear interpolation on log(E), e.g. log
    // interpolation.
    LinearInterpolator<real_type> interpolate_xs(
        {loge_grid[bin], data_.xs[bin]},
        {loge_grid[bin + 1], data_.xs[bin + 1]});
    return interpolate_xs(loge);
}

//---------------------------------------------------------------------------//
} // namespace celeritas
