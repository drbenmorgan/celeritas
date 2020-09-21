//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file PhysicsArrayParams.hh
//---------------------------------------------------------------------------//
#pragma once

#include <vector>
#include "base/DeviceVector.hh"
#include "PhysicsArrayPointers.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Manage 1D arrays of energy-depenent data used by physics classes.
 *
 * TODO: add a PhysicsArrayBuilder factory class that helps assemble the input
 * data (?).
 *
 * TODO: for the purposes of the demo app, this only holds a single array which
 * must be uniformly log-spaced.
 */
class PhysicsArrayParams
{
  public:
    //@{
    //! Type aliases
    //@}

    struct Input
    {
        std::vector<real_type> energy;
        std::vector<real_type> xs;
    };

  public:
    // Construct with input data
    explicit PhysicsArrayParams(const Input& input);

    // Access on-device data
    PhysicsArrayPointers device_pointers() const;

  private:
    UniformGridPointers     log_energy_;
    DeviceVector<real_type> xs_;
};

//---------------------------------------------------------------------------//
} // namespace celeritas
