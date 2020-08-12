//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.hh
//---------------------------------------------------------------------------//
/*!
 * \brief Loads the geant-exporter.root file data into memory
 *
 * GeantImporter loads particle data and physics tables previously saved into
 * a ROOT file by the app/geant-exporter external code.
 *
 * Usage:
 * \code
 *  GeantImporter importer;
 *  importer.load_root_file("/path/to/file.root");
 *  importer.load_particle_data();
 *  importer.load_physics_tables();
 * \endcode
 *
 * These methods will load into memory both particle and physics table data
 * as a vector<GeantParticle> and a vector<GeantPhysicsTable>. The data can
 * be retrieved and copied to a GeantParticle or GeantPhysicsTable via copy
 * functions:
 * \code
 *  importer.copy_GeantParticle(...);
 *  impocopy_GeantPhysicsTable(...);
 * \endcode
 */
//-------------------------------------------------------------------------//

#pragma once

// C++
#include <memory>
#include <string>
#include <vector>

// Project
#include "GeantParticle.hh"
#include "GeantPhysicsTable.hh"
#include "ParticleParams.hh"
#include "ParticleDef.hh"
#include "ParticleMd.hh"

#include "base/Types.hh"
#include "base/Macros.hh"

// ROOT
class TFile;

namespace celeritas
{
//-------------------------------------------------------------------------//

class GeantImporter
{
  private:
    std::unique_ptr<TFile>         root_input_;
    std::vector<std::string>       object_list_;
    std::vector<GeantParticle>     particle_vector_;
    std::vector<GeantPhysicsTable> physics_table_vector_;

    void build_object_list(std::string const root_folder);

  public:
    GeantImporter();
    ~GeantImporter();

    // Loads the ROOT data into memeory
    void load_root_file(std::string const filename);

    void load_particle_data();
    void load_physics_tables();

    std::vector<GeantParticle>     get_vector_GeantParticle();
    std::vector<GeantPhysicsTable> get_vector_GeantPhysicsTable();

    // Print capabilities
    void print_object_list();

    // Copy the data from memory into an object
    bool get_GeantParticle(ssize_type pdg, GeantParticle& g4Particle);
    bool get_GeantPhysicsTable(GeantProcessType process_type,
                               GeantTableType   table_type,
                               GeantProcess     process,
                               GeantModel       model,
                               PDGNumber          pdg,
                               GeantPhysicsTable& physics_table);

    // Construct a ParticleDef
    ParticleDef              particleDef(ssize_type pdg);
    ParticleDef              particleDef(GeantParticle& g4particle);
    std::vector<ParticleDef> particleDef_vector();

    // Construct a ParticleMd
    ParticleMd particleMd(ssize_type pdg);
    ParticleMd particleMd(GeantParticle& g4particle);

    // Construct a ParticleParams
     std::shared_ptr<ParticleParams> get_ParticleParams();

    // Construct a Particle
};

//---------------------------------------------------------------------------//
} // namespace celeritas
