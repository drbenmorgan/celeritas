//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SecondaryAllocatorView.hh
//---------------------------------------------------------------------------//
#pragma once

#include "Secondary.hh"
#include "SecondaryAllocatorPointers.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Stack of secondary particles added by a physics process.
 */
class SecondaryAllocatorView
{
  public:
    //@{
    //! Type aliases
    using size_type          = SecondaryAllocatorPointers::size_type;
    using result_type        = Secondary*;
    using constSpanSecondary = span<const Secondary>;
    using SpanSecondary      = span<Secondary>;
    //@}

  public:
    // Construct with shared data
    explicit inline CELER_FUNCTION
    SecondaryAllocatorView(const SecondaryAllocatorPointers& shared);

    // Allocate space for this many secondaries
    inline CELER_FUNCTION result_type operator()(size_type count);

    // Total storage capacity
    inline CELER_FUNCTION size_type capacity() const;

    // View all active secondaries
    inline CELER_FUNCTION SpanSecondary secondaries();

    // View all active secondaries
    inline CELER_FUNCTION constSpanSecondary secondaries() const;

  private:
    const SecondaryAllocatorPointers& shared_;
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "SecondaryAllocatorView.i.hh"
