//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeoTrackView.i.hh
//---------------------------------------------------------------------------//
#include <VecGeom/navigation/GlobalLocator.h>
#include <VecGeom/navigation/VNavigator.h>
#include "base/ArrayUtils.hh"
#include "detail/VGCompatibility.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Construct from persistent and state data.
 */
CELER_FUNCTION
GeoTrackView::GeoTrackView(const GeoParamsPointers& data,
                           const GeoStatePointers&  stateview,
                           const ThreadId&          id)
    : shared_(data)
    , vgstate_(GeoTrackView::get_nav_state(
          stateview.vgstate, stateview.vgmaxdepth, id))
    , vgnext_(GeoTrackView::get_nav_state(
          stateview.vgnext, stateview.vgmaxdepth, id))
    , pos_(stateview.pos[id.get()])
    , dir_(stateview.dir[id.get()])
    , next_step_(stateview.next_step[id.get()])
{
}

//---------------------------------------------------------------------------//
/*!
 * Construct the state.
 *
 * Expensive. This function should only be called to initialize an event from a
 * starting location and direction. Secondaries will initialize their states
 * from a copy of the parent.
 */
CELER_FUNCTION GeoTrackView& GeoTrackView::operator=(const Initializer_t& init)
{
#ifdef __CUDA_ARCH__
    REQUIRE(false); // VecGeom CUDA global locator is not implemented
#endif

    // Initialize position/direction
    pos_ = init.pos;
    dir_ = init.dir;

    // Set up current state and locate daughter volume.
    vgstate_.Clear();
    const vecgeom::VPlacedVolume* volume         = shared_.world_volume;
    const bool                    contains_point = true;

    // Note that LocateGlobalPoint sets vgstate. If `vgstate_` is outside
    // (including possibly on the outside volume edge), the volume pointer it
    // returns will be null
    vecgeom::GlobalLocator::LocateGlobalPoint(
        volume, detail::to_vector(pos_), vgstate_, contains_point);

    // Set up next state
    vgnext_.Clear();
    next_step_ = celeritas::numeric_limits<real_type>::quiet_NaN();
    return *this;
}

//---------------------------------------------------------------------------//
/*!
 * Find the distance to the next geometric boundary.
 */
CELER_FUNCTION void GeoTrackView::find_next_step()
{
    const vecgeom::LogicalVolume* logical_vol
        = this->volume().GetLogicalVolume();
    CHECK(logical_vol);
    const vecgeom::VNavigator* navigator
        = this->volume().GetLogicalVolume()->GetNavigator();
    CHECK(navigator);

    next_step_
        = navigator->ComputeStepAndPropagatedState(detail::to_vector(pos_),
                                                   detail::to_vector(dir_),
                                                   vecgeom::kInfLength,
                                                   vgstate_,
                                                   vgnext_);
}

//---------------------------------------------------------------------------//
/*!
 * Move to the next boundary.
 */
CELER_FUNCTION void GeoTrackView::move_next_step()
{
    vgstate_ = vgnext_;
    vgnext_.Clear();

    // Move the next step plus an extra fudge distance
    axpy(next_step_, dir_, &pos_);
}

//---------------------------------------------------------------------------//
/*!
 * Get the volume ID in the current cell.
 */
CELER_FUNCTION VolumeId GeoTrackView::volume_id() const
{
    REQUIRE(!this->is_outside());
    return VolumeId{this->volume().id()};
}

//---------------------------------------------------------------------------//
// PRIVATE CLASS FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * Determine the pointer to the navigation state for a particular index.
 *
 * When using the "cuda"-namespace navigation state (i.e., compiling with NVCC)
 * it's necessary to transform the raw data pointer into an index.
 */
CELER_FUNCTION auto
GeoTrackView::get_nav_state(void*                  state,
                            CELER_MAYBE_UNUSED int vgmaxdepth,
                            ThreadId               thread) -> NavState&
{
    REQUIRE(state);
    char* ptr = reinterpret_cast<char*>(state);
#ifdef __NVCC__
    ptr += vecgeom::cuda::NavigationState::SizeOfInstanceAlignAware(vgmaxdepth)
           * thread.get();
#else
    REQUIRE(thread.get() == 0);
#endif
    ENSURE(ptr);
    return *reinterpret_cast<NavState*>(ptr);
}

//---------------------------------------------------------------------------//
/*!
 * Get a reference to the current volume.
 */
CELER_FUNCTION const vecgeom::VPlacedVolume& GeoTrackView::volume() const
{
    const vecgeom::VPlacedVolume* vol_ptr = vgstate_.Top();
    ENSURE(vol_ptr);
    return *vol_ptr;
}

//---------------------------------------------------------------------------//
/*!
 * Check if current step has crossed a boundary, and update navState if
 * necessary
 *
 * //When using the "cuda"-namespace navigation state (i.e., compiling with
 * NVCC)
 * //it's necessary to transform the raw data pointer into an index.
 */
CELER_FORCEINLINE_FUNCTION
bool GeoTrackView::has_same_path()
{
    //#### NOT USING YET THE NEW NAVIGATORS ####//
    // TODO: not using the direction yet here !!
    bool samepath = vecgeom::GlobalLocator::HasSamePath(
        detail::to_vector(pos_), vgstate_, vgnext_);
    if (!samepath)
    {
#ifdef VECGEOM_CACHED_TRANS
        vgnext_->UpdateTopMatrix();
#endif
        vgnext_.CopyTo(&vgstate_);
    }

    return samepath;
}

} // namespace celeritas
