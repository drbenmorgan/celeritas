//----------------------------------*-C++-*----------------------------------//
// Copyright 2022-2023 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file example/corecel/saxpy.hh
//! \brief Interfaces for saxpy calculations with Corecel data/collection types
//---------------------------------------------------------------------------//

#include "corecel/Types.hh"
#include "corecel/data/DeviceVector.hh"

/// @brief saxpy using DeviceVector
/// @param a
/// @param x
/// @param y
/// @param z
void saxpy(celeritas::real_type a,
           celeritas::DeviceVector<celeritas::real_type> const& x,
           celeritas::DeviceVector<celeritas::real_type> const& y,
           celeritas::DeviceVector<celeritas::real_type>& z);
