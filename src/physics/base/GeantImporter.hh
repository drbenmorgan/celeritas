//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.hh
//---------------------------------------------------------------------------//
#ifndef base_GeantImporter_hh
#define base_GeantImporter_hh

// C++
#include <iostream>
#include <string>
#include <vector>
#include <map>

// ROOT
#include "TFile.h"

// Project
#include "GeantParticleDef.hh"
#include "GeantPhysicsTable.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * GeantImporter loads particle data and physics tables previously saved into
 * root files. The particle data is produced by G4ParticleDefinitionExporter
 * and physics tables are exported by the Geant4-Sandbox.
 *
 * Usage:
 * \code
 *  GeantImporter importer;
 *  importer.loadParticleDefRootFile("/path/to/particleDefFile.root");
 *  importer.loadPhysicsTableRootFile("/path/to/physicsTables.root");
 * \endcode
 *
 * These methods will load into memory both particles and physics tables as
 * vector<G4ParticleDef> and map<tableName, G4Physics>. All the data can be
 * accessed by either accessing the vector and map or via copy functions:
 *
 * \code
 *  bool copyParticleDef(int pdg, G4ParticleDef &g4Particle);
 *  bool copyPhysicsTable(std::string physTableName, G4PhysicsTable
 * &physTable); \endcode
 */

class GeantImporter : G4ParticleDef, G4PhysicsTable
{
  protected:
    TFile* rootFile_particleDef;
    TFile* rootFile_physicsTable;

  public:
    std::vector<std::string>              objectsList;
    std::vector<G4ParticleDef>            particleVector;
    std::map<std::string, G4PhysicsTable> physTableMap;

  public:
    GeantImporter();
    ~GeantImporter();

    void loadParticleDefRootFile(std::string const filename);
    void loadPhysicsTableRootFile(std::string const filename);

    bool copyParticleDef(int pdg, G4ParticleDef& g4Particle);
    bool copyPhysicsTable(std::string physTableName, G4PhysicsTable& physTable);

    void printObjectsList();
    void printParticleInfo(int pdg);
    void printPhysicsTable(std::string physTableName);
    void printPhysTableNames();

  private:
    void buildObjectsList(TFile* rootFile);
    void loadPhysicsTablesIntoMemory();
    void loadParticleDefsIntoMemory();
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#endif // base_GeantImporter_hh
