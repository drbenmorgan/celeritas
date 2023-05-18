//----------------------------------*-C++-*----------------------------------//
// Copyright 2022-2023 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file example/corecel/collection.cc
//---------------------------------------------------------------------------//

#include "corecel/data/Collection.hh"

#include <iostream>

#include "corecel/Types.hh"
#include "corecel/data/CollectionBuilder.hh"
#include "corecel/data/Ref.hh"

// Basic item
struct MyPoint
{
    celeritas::real_type x{};
    celeritas::real_type y{};
    celeritas::real_type z{};
};

std::ostream& operator<<(std::ostream& os, MyPoint const& p)
{
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}

// Basic ID type
using MyPointId = celeritas::ItemId<MyPoint>;

// The collection
using MyPointCollection = celeritas::Collection<MyPoint,
                                                celeritas::Ownership::value,
                                                celeritas::MemSpace::host>;

// View by ref
using MyPointCollectionView
    = celeritas::Collection<MyPoint,
                            celeritas::Ownership::reference,
                            celeritas::MemSpace::host>;

int main()
{
    // Can build a collection on host with a builder
    MyPointCollection host_col;
    auto builder = celeritas::make_builder(&host_col);

    for (auto i : celeritas::Range<celeritas::real_type>(10))
    {
        builder.push_back(MyPoint{i, i + 1, i + 2});
    }

    // Should now print
    for (auto const& p : host_col[MyPointCollection::AllItemsT{}])
    {
        std::cout << p << std::endl;
    }

    // Modify
    host_col[MyPointId{2}] = {
        42.,
        42.,
        42,
    };

    // Check
    for (auto const& p : host_col[MyPointCollection::AllItemsT{}])
    {
        std::cout << p << std::endl;
    }

    // make a view in a separate scope
    {
        // See also the Ref.hh header, though that is more for structs that
        // hold Collections
        MyPointCollectionView host_view{host_col};
        // Can't do the following if the view has const_reference ownership,
        // even for non-const collection const ref to MyCollectionView that
        // owns non-const refs also disallows this
        host_view[MyPointId{4}] = {
            42.,
            42.,
            42,
        };
    }

    // Has the viewed collection changed?
    for (auto const& p : host_col[MyPointCollection::AllItemsT{}])
    {
        std::cout << p << std::endl;
    }
}
