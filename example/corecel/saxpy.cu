//----------------------------------*-C++-*----------------------------------//
// Copyright 2022-2023 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file example/corecel/saxpy.cu
//! \brief Interface/kernel pairs for saxpy calculation with Corecel
//! data/container types
//---------------------------------------------------------------------------//

#include "saxpy.hh"

#include <iostream>
#include <vector>

#include "corecel/cont/Span.hh"
#include "corecel/data/DeviceVector.hh"
#include "corecel/sys/KernelParamCalculator.device.hh"

__global__ void
saxpy_kernel(celeritas::real_type const a,
             celeritas::Span<celeritas::real_type const> const x,
             celeritas::Span<celeritas::real_type const> const y,
             celeritas::Span<celeritas::real_type> z)
{
    // Get ThreadID
    auto tid = celeritas::KernelParamCalculator::thread_id();
    if (tid.get() >= x.size())
        return;

    auto i = tid.get();
    z[i] = a * x[i] + y[i];
}

void saxpy(celeritas::real_type a,
           celeritas::DeviceVector<celeritas::real_type> const& x,
           celeritas::DeviceVector<celeritas::real_type> const& y,
           celeritas::DeviceVector<celeritas::real_type>& z)
{
    // 4. Launch kernel
    CELER_LAUNCH_KERNEL(saxpy,
                        celeritas::device().default_block_size(),
                        x.size(),
                        a,
                        x.device_ref(),
                        y.device_ref(),
                        z.device_ref());
}
