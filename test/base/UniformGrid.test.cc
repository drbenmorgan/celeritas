//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file UniformGrid.test.cc
//---------------------------------------------------------------------------//
#include "base/UniformGridView.hh"

#include "gtest/Main.hh"
#include "gtest/Test.hh"

using celeritas::UniformGridView;

//---------------------------------------------------------------------------//
// TEST HARNESS
//---------------------------------------------------------------------------//

class UniformGridTest : public celeritas::Test
{
  protected:
    void SetUp() override
    {
        host_ptrs.size  = 3; //!< Number of grid points (2 bins!)
        host_ptrs.front = 1.0;
        host_ptrs.delta = 1.5;
    }

    celeritas::UniformGridPointers host_ptrs;
};

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST_F(UniformGridTest, accessors)
{
    UniformGridView grid(host_ptrs);
    EXPECT_EQ(3, grid.size());
    EXPECT_DOUBLE_EQ(1.0, grid.front());
    EXPECT_DOUBLE_EQ(1.0 + 1.5 * 2, grid.back());
    EXPECT_DOUBLE_EQ(1.0, grid[0]);
    EXPECT_DOUBLE_EQ(1.0 + 1.5 * 2, grid[2]);
}

TEST_F(UniformGridTest, find)
{
    UniformGridView grid(host_ptrs);
#if CELERITAS_DEBUG
    EXPECT_THROW(grid.find(0.99999), celeritas::DebugError);
#endif
    EXPECT_EQ(0, grid.find(1.0));
    EXPECT_EQ(0, grid.find(2.49999));
    EXPECT_EQ(1, grid.find(2.5));
    EXPECT_EQ(1, grid.find(3.99999));
#if CELERITAS_DEBUG
    EXPECT_THROW(grid.find(4.0), celeritas::DebugError);
    EXPECT_THROW(grid.find(4.0 + 0.00001), celeritas::DebugError);
#endif
}
