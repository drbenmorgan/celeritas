//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsArrayParams.cc
//---------------------------------------------------------------------------//
#include "PhysicsArrayParams.hh"

#include <algorithm>
#include <cmath>
#include "base/Range.hh"
#include "base/SoftEqual.hh"
#include "base/UniformGridView.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with input data.
 */
PhysicsArrayParams::PhysicsArrayParams(const Input& input)
    : xs_(input.xs.size())
{
    REQUIRE(input.energy.size() >= 2);
    REQUIRE(input.energy.front() > 0);
    REQUIRE(std::is_sorted(input.energy.begin(), input.energy.end()));
    REQUIRE(input.xs.size() == input.energy.size());
    REQUIRE(std::all_of(
        input.xs.begin(), input.xs.end(), [](real_type v) { return v >= 0; }));

    // Calculate uniform-in-logspace energy grid
    log_energy_.size  = input.energy.size();
    log_energy_.front = std::log(input.energy.front());
    log_energy_.delta = (std::log(input.energy.back()) - log_energy_.front)
                        / (log_energy_.size - 1);
#if CELERITAS_DEBUG
    {
        // Test soft equivalence between log energy grid and input energy to
        // make sure all the points are uniformly spaced
        celeritas::SoftEqual<>     soft_eq(1e-10);
        celeritas::UniformGridView log_energy(log_energy_);
        for (auto i : range(input.energy.size()))
        {
            REQUIRE(soft_eq(std::log(input.energy[i]), log_energy[i]));
        }
    }
#endif

    // Copy xs values to device
    xs_.copy_to_device(make_span(input.xs));
}

//---------------------------------------------------------------------------//
/*!
 * Access on-device data.
 */
PhysicsArrayPointers PhysicsArrayParams::device_pointers() const
{
    PhysicsArrayPointers result;
    result.log_energy = log_energy_;
    result.xs         = xs_.device_pointers();
    return result;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
