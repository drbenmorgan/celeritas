//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file VGAccessor.hh
//---------------------------------------------------------------------------//
#ifndef geometry_VGAccessor_hh
#define geometry_VGAccessor_hh

#include "VGStateView.hh"
#include "VGParamsView.hh"
#include "Types.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Obtain minimal information from the geometry state.
 */
class VGAccessor
{
  public:
    struct Initializer_t
    {
        Real3 pos;
        Real3 dir;
    };

  public:
    // Construct from persistent and state data
    inline CELER_FUNCTION
    VGAccessor(const VGStateView& stateview, const ThreadId& id);

    //@{
    //! State accessors
    CELER_FUNCTION const Real3& pos() const { return pos_; }
    CELER_FUNCTION const Real3& dir() const { return dir_; }
    //@}

  private:
    //@{
    //! Referenced thread-local data
    const Real3& pos_;
    const Real3& dir_;
    //@}
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "VGAccessor.i.hh"

#endif // geometry_VGAccessor_hh
