//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantImporter.cc
//---------------------------------------------------------------------------//
#include "GeantImporter.hh"

#include <iomanip>
#include <iostream>

// ROOT
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Default constructor and destructor
 */
GeantImporter::GeantImporter()  = default;
GeantImporter::~GeantImporter() = default;

//---------------------------------------------------------------------------//
/*!
 * Loads the data from the particleData.root file into memory as a vector
 */
void GeantImporter::load_root_file(std::string const filename)
{
    this->root_input_ = std::make_unique<TFile>(filename.c_str(), "open");
}

//---------------------------------------------------------------------------//
/*!
 * Copies a particle from the vector into a GeantParticleDef
 */
bool GeantImporter::copy_geantParticleDef(int pdg, GeantParticleDef& g4Particle)
{
    for (auto aParticle : this->particleVector_)
    {
        if (aParticle.pdg() == pdg)
        {
            g4Particle = aParticle;
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Copies a physics table from the map into a GeantPhysicsTable
 */
bool GeantImporter::copy_geantPhysicsTable(std::string        physTableName,
                                           GeantPhysicsTable& physTable)
{
    for (auto thisPair : this->physTableMap_)
    {
        if (thisPair.first == physTableName)
        {
            physTable = thisPair.second;
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Prints the loaded objects list, useful to check the physics tables names
 */
void GeantImporter::printObjectsList()
{
    for (auto name : this->objectsList_)
    {
        std::cout << name << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Finds the particle using its PDG and prints all its GeantParticleDef data
 */
void GeantImporter::printParticleInfo(int pdg)
{
    GeantParticleDef particle;

    if (!copy_geantParticleDef(pdg, particle))
    {
        std::cout << "Particle not found" << std::endl;
        return;
    }

    std::cout << "-----------------------" << std::endl;
    std::cout << particle.name() << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "pdg       : " << particle.pdg() << std::endl;
    std::cout << "mass      : " << particle.mass() << std::endl;
    std::cout << "charge    : " << particle.charge() << std::endl;
    std::cout << "spin      : " << particle.spin() << std::endl;
    std::cout << "lifetime  : " << particle.lifetime() << std::endl;
    std::cout << "is_stable : " << particle.is_stable() << std::endl;
    std::cout << "-----------------------" << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Prints all the data from a given GeantPhysicsTable by providing its name
 */
/*
void GeantImporter::printPhysicsTable(std::string physTableName)
{
    std::cout << std::setprecision(3);
    std::cout << std::fixed;
    std::cout << std::scientific;

    GeantPhysicsTable aTable;

    if (!copyPhysicsTable(physTableName, aTable))
    {
        std::cout << "Physics table not found" << std::endl;
        return;
    }

    std::cout << physTableName << std::endl;
    std::cout << " | tableSize: " << aTable.size() << std::endl;

    for (int i = 0; i < aTable.size(); i++)
    {
        std::cout << " |" << std::endl;
        std::cout << " | --------------------------" << std::endl;
        std::cout << " | ENTRY " << i << std::endl;
        std::cout << " | --------------------------" << std::endl;
        std::cout << " | edgeMin       : ";
        std::cout << aTable.binVector_.at(i).at(0) << std::endl;
        std::cout << " | edgeMax       : ";
        std::cout << aTable.binVector_.at(i).at(aTable.binVector_.at(i).size())
<< std::endl; std::cout << " | numberOfNodes : "; std::cout <<
aTable.binVector_.at(i).size() << std::endl; std::cout << " | vectorType    :
"; std::cout << aTable.vector_type_.at(i) << std::endl; std::cout << " |
binVector      dataVector" << std::endl;

        for (int j = 0; j < aTable.binVector_.at(i).size(); j++)
        {
            std::cout << " | | ";
            std::cout << std::left << std::setw(12);
            std::cout << std::setfill(' ');
            std::cout << aTable.binVector_.at(i).at(j);
            std::cout << " | ";
            std::cout << aTable.dataVector_.at(i).at(j);
            std::cout << std::endl;
        }
    }
}
*/

//---------------------------------------------------------------------------//
/*!
 * Prints all the physics table names found in the ROOT file
 */
void GeantImporter::printPhysicsTableNames()
{
    for (auto aTable : this->physTableMap_)
    {
        std::cout << aTable.first << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Returns a ParticleDef type by providing a PDG number
 */
ParticleDef GeantImporter::particleDef(ssize_type pdg)
{
    ParticleDef      particle;
    GeantParticleDef g4particle;

    copy_geantParticleDef(pdg, g4particle);
    particle = particleDef(g4particle);

    // TODO: Safeguard if no particle is found
    return particle;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a ParticleDef type by providing a GeantParticleDef type
 */
ParticleDef GeantImporter::particleDef(GeantParticleDef& g4particle)
{
    ParticleDef particle;

    particle.mass   = g4particle.mass();
    particle.charge = g4particle.charge();

    if (g4particle.is_stable())
    {
        particle.decay_constant = ParticleDef::stable_decay_constant();
    }

    else
    {
        particle.decay_constant = 1. / g4particle.lifetime();
    }

    return particle;
}

//---------------------------------------------------------------------------//
std::vector<ParticleDef> GeantImporter::particleDefVector()
{
    std::vector<ParticleDef> particleVec;
    ParticleDef              particle;

    for (auto gParticleDef : this->particleVector_)
    {
        particle = this->particleDef(gParticleDef);
        particleVec.push_back(particle);
    }

    return particleVec;
}

//---------------------------------------------------------------------------//
/*!
 * Creates a list of all the object names found in the ROOT file
 */
void GeantImporter::build_objects_list(std::string const root_folder)
{
    TDirectory* directory
        = (TDirectory*)this->root_input_->Get(root_folder.c_str());

    // Getting list of keys -- i.e. list of elements in the root input
    TList* list = directory->GetListOfKeys();

    // Getting list iterator
    TIter iter(list->MakeIterator());

    // Cleaning vector
    this->objectsList_.clear();

    // Looping over the objects found in rootInput
    while (TObject* object = iter())
    {
        // Getting the object's name and type
        TKey*       key     = (TKey*)object;
        std::string keyName = key->GetName();
        std::string keyType = key->GetClassName();

        // Safeguard to avoid reading a non-tree object
        // Just in case we add something else to the file in the future
        if (keyType != "TTree")
            continue;

        this->objectsList_.push_back(keyName);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<GeantParticleDef>.
 */
void GeantImporter::load_particleDefs()
{
    build_objects_list("particles");

    this->particleVector_.clear();

    GeantParticleDef thisParticle;
    std::string*     branchName = new std::string;

    for (auto particleName : this->objectsList_)
    {
        std::string particle_path_name = "particles/" + particleName;

        TTree* treeParticle
            = (TTree*)this->root_input_->Get(particle_path_name.c_str());

        treeParticle->SetBranchAddress("name", &branchName);

        treeParticle->GetEntry(0);

        std::string thisName   = *branchName;
        ssize_type  thisPdg    = treeParticle->GetLeaf("pdg")->GetValue();
        real_type   thisMass   = treeParticle->GetLeaf("mass")->GetValue();
        real_type   thisCharge = treeParticle->GetLeaf("charge")->GetValue();
        real_type   thisSpin   = treeParticle->GetLeaf("spin")->GetValue();
        real_type thisLifetime = treeParticle->GetLeaf("lifetime")->GetValue();
        bool thisIsStable = treeParticle->GetLeaf("is_stable")->GetValue();

        thisParticle(thisName,
                     thisPdg,
                     thisMass,
                     thisCharge,
                     thisSpin,
                     thisLifetime,
                     thisIsStable);

        this->particleVector_.push_back(thisParticle);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<GeantParticleDef>.
 */
void GeantImporter::load_physicsTables()
{
    build_objects_list("tables");

    GeantPhysicsTable  pTable;
    GeantPhysicsVector pVector;

    for (auto table_name : objectsList_)
    {
        std::string table_path_name = "tables/" + table_name;
        // Creating a tree pointer and getting the tree
        TTree* tree = (TTree*)this->root_input_->Get(table_path_name.c_str());

        // For accessing the tree members
        std::vector<real_type>* read_energy   = new std::vector<real_type>;
        std::vector<real_type>* read_xs_eloss = new std::vector<real_type>;

        tree->SetBranchAddress("energy", &read_energy);
        tree->SetBranchAddress("xs_eloss", &read_xs_eloss);

        // Looping over the tree entries
        for (int i = 0; i < tree->GetEntries(); i++)
        {
            // Clearing energy and xs_eloss vectors
            pVector.energy.clear();
            pVector.xs_eloss.clear();

            // Fetching tree entry
            tree->GetEntry(i);

            // Fetching the values of each leaf
            pVector.vector_type
                = (GeantPhysicsVectorType)tree->GetLeaf("vectorType")->GetValue();

            // Looping over binVector and dataVector
            for (int j = 0; j < read_energy->size(); j++)
            {
                pVector.energy.push_back(read_energy->at(j));
                pVector.xs_eloss.push_back(read_xs_eloss->at(j));
            }

            pTable.physics_vectors.push_back(pVector);
        }
        this->physTableMap_.emplace(std::make_pair(table_name, pTable));
        //std::cout << "Loaded " << table_name << std::endl; 
    }
}

//---------------------------------------------------------------------------//
} // namespace celeritas
