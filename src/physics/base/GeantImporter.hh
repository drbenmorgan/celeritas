//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.hh
//---------------------------------------------------------------------------//
/*!
* GeantImporter loads particle data and physics tables previously saved into
* root files. The particle data is produced by GeantParticleDefinitionExporter
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
* vector<GeantParticleDef> and map<tableName, G4Physics>. All the data can be
* accessed by either accessing the vector and map or via copy functions:
*
* \code
*  bool copyParticleDef(int pdg, GeantParticleDef &g4Particle);
*  bool copyPhysicsTable(std::string physTableName, GeantPhysicsTable
* &physTable); \endcode
*/
//-------------------------------------------------------------------------//

#pragma once

// C++
#include <memory>
#include <string>
#include <vector>
#include <map>

// Project
#include "GeantParticleDef.hh"
#include "GeantPhysicsTable.hh"

// Root forward declaration
class TFile;

namespace celeritas
{
//-------------------------------------------------------------------------//

class GeantImporter
{
  public:
    GeantImporter();
    ~GeantImporter();

    void loadParticleDefRootFile(std::string const filename);
    void loadPhysicsTableRootFile(std::string const filename);

    bool copyParticleDef(int pdg, GeantParticleDef& g4Particle);
    bool copyPhysicsTable(std::string        physTableName,
                          GeantPhysicsTable& physTable);

    void printObjectsList();
    void printParticleInfo(int pdg);
    void printPhysicsTable(std::string physTableName);
    void printPhysTableNames();

  private:
    void buildObjectsList(TFile* rootFile);
    void loadPhysicsTablesIntoMemory();
    void loadParticleDefsIntoMemory();

  private:
    std::vector<std::string>                 objectsList;
    std::vector<GeantParticleDef>            particleVector;
    std::map<std::string, GeantPhysicsTable> physTableMap;

    std::unique_ptr<TFile> rootFile_particleDef;
    std::unique_ptr<TFile> rootFile_physicsTable;
};

//---------------------------------------------------------------------------//
} // namespace celeritas

