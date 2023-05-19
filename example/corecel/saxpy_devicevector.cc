//----------------------------------*-C++-*----------------------------------//
// Copyright 2022-2023 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file example/corecel/saxpy.cc
//! \brief Demonstrates use of DeviceVector for calculation of saxpy
//---------------------------------------------------------------------------//

#include "saxpy.hh"

#include <iostream>
#include <numeric>
#include <vector>

#include "corecel/Assert.hh"
#include "corecel/Types.hh"
#include "corecel/cont/Range.hh"
#include "corecel/data/DeviceVector.hh"
#include "corecel/sys/Device.hh"

int main()
{
    using HostVector = std::vector<celeritas::real_type>;
    using DeviceVector = celeritas::DeviceVector<celeritas::real_type>;

    // 1. Host input data
    constexpr celeritas::size_type N{1 << 20};
    constexpr celeritas::real_type a = 3.14;
    HostVector x_h(N), y_h(N), expected_h(N);

    for (auto const v : celeritas::Range<int>(N))
    {
        x_h[v] = v * 1.0;
        y_h[v] = v * 2.0;
        expected_h[v] = a * x_h[v] + y_h[v];
    }

    // Initialize GPU
    celeritas::activate_device(celeritas::Device(0));

    // Copy to Device
    DeviceVector x_d(N), y_d(N), result_d(N);
    x_d.copy_to_device(celeritas::make_span(x_h));
    y_d.copy_to_device(celeritas::make_span(y_h));

    // Calculate
    saxpy(a, x_d, y_d, result_d);

    // Get result on host
    HostVector result_h(result_d.size());
    result_d.copy_to_host(celeritas::make_span(result_h));

    // Basic checks
    CELER_ENSURE(result_h.size() == expected_h.size());
    auto result_sum = std::accumulate(result_h.begin(), result_h.end(), 0.0);
    auto expected_sum
        = std::accumulate(expected_h.begin(), expected_h.end(), 0.0);

    CELER_ENSURE((result_sum - expected_sum) == 0.0);
}
