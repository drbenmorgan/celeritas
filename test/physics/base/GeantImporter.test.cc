//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.test.cc
//---------------------------------------------------------------------------//
#include "physics/base/GeantImporter.hh"

#include "gtest/Main.hh"
#include "gtest/Test.hh"
#include "gmock/gmock.h"

#include <string>

using celeritas::GeantImporter;
using celeritas::GeantParticleDef;
using celeritas::GeantPhysicsTable;

//---------------------------------------------------------------------------//
// TEST HARNESS
//---------------------------------------------------------------------------//

class GeantImporterTest : public celeritas::Test
{
  protected:
    void SetUp() override
    {}
    
    GeantImporter importer_;

    std::string testPath_ = "/Users/4s2/celeritas-project/celeritas/test";
    std::string particleDefFile_
    = testPath_ + "/physics/base/data/particleDefinitionTable.root";
    std::string physicsTablesFile_
    = testPath_ + "/physics/base/data/physicsTables.root";
};

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particle)
{
    importer_.loadParticleDefRootFile(particleDefFile_);
    GeantParticleDef particle;
       
    EXPECT_TRUE(importer_.copyParticleDef(11, particle));
    EXPECT_SOFT_EQ(0.510998910, particle.mass());
    EXPECT_EQ(-1 , particle.charge());
    EXPECT_EQ(0.5, particle.spin());
    EXPECT_EQ(-1 , particle.lifetime());
    EXPECT_TRUE(particle.isStable());
}

TEST_F(GeantImporterTest, physicsTable)
{
    importer_.loadPhysicsTableRootFile(physicsTablesFile_);
    GeantPhysicsTable physTable;
        
    EXPECT_TRUE(importer_.copyPhysicsTable("DEDX_eIoni_eMinus", physTable));
}

