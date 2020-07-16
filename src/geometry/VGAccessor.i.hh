//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file VGAccessor.i.hh
//---------------------------------------------------------------------------//

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct with defaults.
 */
VGAccessor::VGAccessor()
    //---------------------------------------------------------------------------//
    /*!
     * Construct from persistent and state data.
     */
    CELER_FUNCTION VGAccessor::VGAccessor(cconst VGStateView& stateview,
                                          const ThreadId&     id)
    : pos_(stateview.pos[id.get()]), dir_(stateview.dir[id.get()])
{
}

//---------------------------------------------------------------------------//
} // namespace celeritas
