//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeoTrackView.hh
//---------------------------------------------------------------------------//
#pragma once

#include <VecGeom/volumes/PlacedVolume.h>
#include <VecGeom/navigation/NavigationState.h>

#include "base/Macros.hh"
#include "base/NumericLimits.hh"
#include "GeoStatePointers.hh"
#include "GeoParamsPointers.hh"
#include "Types.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Operate on the device with shared (persistent) data and local state.
 *
 * \code
    GeoTrackView geom(vg_view, vg_state_view, thread_id);
   \endcode
 */
class GeoTrackView
{
  public:
    struct Initializer_t
    {
        Real3 pos;
        Real3 dir;
    };

  public:
    // Construct from persistent and state data
    inline CELER_FUNCTION GeoTrackView(const GeoParamsPointers& data,
                                       const GeoStatePointers&  stateview,
                                       const ThreadId&          id);

    // Initialize the state
    inline CELER_FUNCTION GeoTrackView& operator=(const Initializer_t& init);
    // Find the distance to the next boundary
    inline CELER_FUNCTION void find_next_step();
    // Move to the next boundary
    inline CELER_FUNCTION void move_next_step();

    // Check for boundary cross in step, update next state if needed
    CELER_FORCEINLINE_FUNCTION bool has_same_path();

    //@{
    //! State accessors
    CELER_FUNCTION const Real3& pos() const { return pos_; }
    CELER_FUNCTION const Real3&    dir() const { return dir_; }
    CELER_FUNCTION real_type       next_step() const { return next_step_; }
    //@}

    //@{
    //! State modifiers via non-const references
    CELER_FUNCTION Real3& pos() { return pos_; }
    CELER_FUNCTION Real3& dir() { return dir_; }
    CELER_FUNCTION real_type& next_step() { return next_step_; }
    //@}

    /*!
     * Get the volume ID in the current cell.
     */
    inline CELER_FUNCTION VolumeId volume_id() const;

    /*!
     * Return whether the track is inside or outside the valid geometry region.
     */
    CELER_FUNCTION bool is_outside() const { return vgstate_.IsOutside(); }

  private:
    //@{
    //! Type aliases
    using Volume   = vecgeom::VPlacedVolume;
    using NavState = vecgeom::NavigationState;
    //@}

    //! Shared/persistent geometry data
    const GeoParamsPointers& shared_;

    //@{
    //! Referenced thread-local data
    NavState&  vgstate_;
    NavState&  vgnext_;
    Real3&     pos_;
    Real3&     dir_;
    real_type& next_step_;
    //@}

  private:
    // Get a reference to the state from a NavStatePool's pointer
    static inline CELER_FUNCTION NavState&
                                 get_nav_state(void* state, int vgmaxdepth, ThreadId thread);

    // Get a reference to the current volume
    inline CELER_FUNCTION const Volume& volume() const;
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "GeoTrackView.i.hh"
