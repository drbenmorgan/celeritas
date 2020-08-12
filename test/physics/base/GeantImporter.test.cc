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

using celeritas::GeantImporter;
using celeritas::GeantModel;
using celeritas::GeantParticle;
using celeritas::GeantPhysicsTable;
using celeritas::GeantProcess;
using celeritas::GeantProcessType;
using celeritas::GeantTableType;

using celeritas::ParticleDef;
using celeritas::ParticleDefId;
using celeritas::ParticleParams;
using celeritas::PDGNumber;

//---------------------------------------------------------------------------//
// TEST HARNESS
//---------------------------------------------------------------------------//

class GeantImporterTest : public celeritas::Test
{
  protected:
    void SetUp() override {}

    GeantImporter importer_;

    std::string testPath_  = "/Users/4s2/celeritas-project/celeritas/build";
    std::string root_file_ = testPath_ + "/app/geant-exporter-data.root";
};

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particle_electron)
{
    importer_.load_root_file(root_file_);
    importer_.load_particle_data();

    GeantParticle particle;

    EXPECT_TRUE(importer_.get_GeantParticle(11, particle));
    EXPECT_SOFT_EQ(0.510998910, particle.mass());
    EXPECT_EQ(-1, particle.charge());
    EXPECT_EQ(0.5, particle.spin());
    EXPECT_EQ(-1, particle.lifetime());
    EXPECT_TRUE(particle.is_stable());
}
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, physics_table_KN)
{
    importer_.load_root_file(root_file_);
    importer_.load_physics_tables();

    GeantPhysicsTable table;

    EXPECT_TRUE(
        importer_.get_GeantPhysicsTable(GeantProcessType::electromagnetic,
                                        GeantTableType::Lambda,
                                        GeantProcess::compt,
                                        GeantModel::KleinNishina,
                                        celeritas::pdg::gamma(),
                                        table));
}
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particle_def_electron)
{
    importer_.load_root_file(root_file_);
    importer_.load_particle_data();

    ParticleDef particle = importer_.particleDef(11);

    EXPECT_SOFT_EQ(0.510998910, particle.mass);
    EXPECT_EQ(-1, particle.charge);
    EXPECT_EQ(0, particle.decay_constant);
}
//---------------------------------------------------------------------------//

TEST_F(GeantImporterTest, particle_params_electron)
{
    importer_.load_root_file(root_file_);
    importer_.load_particle_data();

    std::shared_ptr<ParticleParams> particle_params;
    particle_params = importer_.get_ParticleParams();

    EXPECT_EQ(20, particle_params->size());
    EXPECT_GE(particle_params->find(PDGNumber(11)).get(), 0);

    ParticleDefId electron_id = particle_params->find(PDGNumber(11));
    ParticleDef electron = particle_params->get(electron_id);

    EXPECT_SOFT_EQ(0.510998910, electron.mass);
    EXPECT_EQ(-1, electron.charge);
    EXPECT_EQ(0, electron.decay_constant);
}
