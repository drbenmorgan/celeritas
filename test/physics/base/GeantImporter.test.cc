//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.test.cc
//---------------------------------------------------------------------------//
#include "physics/base/GeantImporter.hh"
#include "physics/base/ParticleDef.hh"
#include "base/Span.hh"

#include "gtest/Main.hh"
#include "gtest/Test.hh"
#include "gmock/gmock.h"

#include <string>

using celeritas::GeantImporter;
using celeritas::GeantParticleDef;
using celeritas::GeantPhysicsTable;
using celeritas::ParticleDef;

//---------------------------------------------------------------------------//
// TEST HARNESS
//---------------------------------------------------------------------------//

class GeantImporterTest : public celeritas::Test
{
  protected:
    void SetUp() override {}

    GeantImporter importer_;

    std::string testPath_ = "/Users/4s2/celeritas-project/celeritas/test";
    std::string particleDefFile_
        = testPath_ + "/physics/base/data/particleDefinitionTable.root";
    std::string physicsTablesFile_ = testPath_
                                     + "/physics/base/data/physicsTables.root";
};

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particle_electron)
{
    importer_.loadParticleDefRootFile(particleDefFile_);
    GeantParticleDef particle;

    EXPECT_TRUE(importer_.copyParticleDef(11, particle));
    EXPECT_SOFT_EQ(0.510998910, particle.mass());
    EXPECT_EQ(-1, particle.charge());
    EXPECT_EQ(0.5, particle.spin());
    EXPECT_EQ(-1, particle.lifetime());
    EXPECT_TRUE(particle.isStable());
}

//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, physicsTable)
{
    importer_.loadPhysicsTableRootFile(physicsTablesFile_);
    GeantPhysicsTable physTable;

    EXPECT_TRUE(importer_.copyPhysicsTable("DEDX_eIoni_eMinus", physTable));
}

//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particleDef_object)
{
    importer_.loadParticleDefRootFile(particleDefFile_);

    ParticleDef particle = importer_.particleDef(11);

    EXPECT_SOFT_EQ(0.510998910, particle.mass);
    EXPECT_EQ(-1, particle.charge);
    EXPECT_EQ(-1, particle.decay_constant);
}

//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particleDefVector)
{
    importer_.loadParticleDefRootFile(particleDefFile_);

    std::vector<ParticleDef> particleDefVector = importer_.particleDefVector();

    celeritas::span<ParticleDef> particleDefSpan;
    /*
    for (auto particle : particleDefVector)
    {
        std::cout << "mass    : " << particle.mass << std::endl;
        std::cout << "charge  : " << particle.charge << std::endl;
        std::cout << "decay ct: " << particle.decay_constant << std::endl;
        std::cout << std::endl;
    }
    */
}