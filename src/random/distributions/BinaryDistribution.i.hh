//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file BinaryDistribution.i.hh
//---------------------------------------------------------------------------//

#include "base/Assert.hh"
#include "GenerateCanonical.hh"

#include <iostream>
using std::cout;

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with the probability of returning true.
 */
CELER_FUNCTION BinaryDistribution::BinaryDistribution(real_type p_true)
    : p_true_(p_true)
{
    REQUIRE(p_true >= 0.0 && p_true <= 1.0);
}

//---------------------------------------------------------------------------//
/*!
 * Construct with the UNnormalized probability of returning true or false
 */
CELER_FUNCTION BinaryDistribution::BinaryDistribution(real_type scaled_true,
                                                      real_type scaled_false)
    : p_true_(scaled_true / (scaled_true + scaled_false))
{
    REQUIRE(scaled_true > 0 || scaled_false > 0);
    REQUIRE(scaled_true >= 0.0 && scaled_false >= 0.0);
}

//---------------------------------------------------------------------------//
/*!
 * Construct with the probability of returning true.
 */
template<class Generator>
CELER_FUNCTION auto BinaryDistribution::operator()(Generator& rng)
    -> result_type
{
    GenerateCanonical<Generator, real_type> sample_uniform;
    return sample_uniform(rng) < p_true_;
}

//---------------------------------------------------------------------------//
} // namespace celeritas
