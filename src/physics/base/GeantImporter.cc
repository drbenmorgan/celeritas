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
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<GeantParticle>.
 */
void GeantImporter::load_particle_data()
{
    build_object_list("particles");

    this->particle_vector_.clear();

    GeantParticle thisParticle;
    std::string*  branchName = new std::string;

    for (auto particleName : this->object_list_)
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

        this->particle_vector_.push_back(thisParticle);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Loops over the objects list created by buildObjectsList() to create a
 * vector<GeantParticle>.
 */
void GeantImporter::load_physics_tables()
{
    build_object_list("tables");

    for (auto table_name : object_list_)
    {
        GeantPhysicsTable               pTable;
        GeantPhysicsVector              pVector;
        std::vector<GeantPhysicsVector> pVectors;

        std::string table_path_name = "tables/" + table_name;

        // Creating a tree pointer and getting the tree
        TTree* tree = (TTree*)this->root_input_->Get(table_path_name.c_str());

        // For accessing the tree members
        GeantProcessType      process_type;
        std::string*          process_type_name = new std::string;
        std::string*          table_type        = new std::string;
        std::string*          process           = new std::string;
        std::string*          model             = new std::string;
        std::string*          particle          = new std::string;
        std::vector<ssize_t>* vector_type       = new std::vector<ssize_t>;
        std::vector<std::vector<real_type>>* read_energy
            = new std::vector<std::vector<real_type>>;
        std::vector<std::vector<real_type>>* read_xs_eloss
            = new std::vector<std::vector<real_type>>;

        tree->SetBranchAddress("processTypeName", &process_type_name);
        tree->SetBranchAddress("tableType", &table_type);
        tree->SetBranchAddress("process", &process);
        tree->SetBranchAddress("model", &model);
        tree->SetBranchAddress("particle", &particle);
        tree->SetBranchAddress("vectorType", &vector_type);
        tree->SetBranchAddress("energy", &read_energy);
        tree->SetBranchAddress("xs_eloss", &read_xs_eloss);

        // Fetching one time only data
        tree->GetEntry(0);

        // Looping over the tree vector data
        for (int i = 0; i < vector_type->size(); i++)
        {
            pVector.vector_type = (GeantPhysicsVectorType)vector_type->at(i);

            // Clearing energy and xs_eloss vectors
            pVector.energy.clear();
            pVector.xs_eloss.clear();

            // Looping over binVector and dataVector
            for (int j = 0; j < read_energy->at(i).size(); j++)
            {
                pVector.energy.push_back(read_energy->at(i).at(j));
                pVector.xs_eloss.push_back(read_xs_eloss->at(i).at(j));
            }

            // pTable.physics_vectors.push_back(pVector);
            pVectors.push_back(pVector);
        }

        pTable((GeantProcessType)tree->GetLeaf("processType")->GetValue(),
               pTable.get_table_type(*table_type),
               pTable.get_process(*process),
               pTable.get_model(*model),
               *particle,
               tree->GetLeaf("pdg")->GetValue(),
               pVectors);

        this->physics_table_vector_.push_back(pTable);
    }
}

//---------------------------------------------------------------------------//
/*!
 * Access to this->particle_vector_
 */
std::vector<GeantParticle> GeantImporter::get_vector_GeantParticle()
{
    return this->particle_vector_;
}

//---------------------------------------------------------------------------//
/*!
 * Access to this->physics_table_vector_
 */
std::vector<GeantPhysicsTable> GeantImporter::get_vector_GeantPhysicsTable()
{
    return this->physics_table_vector_;
}

//---------------------------------------------------------------------------//
/*!
 * Prints the loaded objects list
 */
void GeantImporter::print_object_list()
{
    for (auto name : this->object_list_)
    {
        std::cout << name << std::endl;
    }
}

//---------------------------------------------------------------------------//
/*!
 * Copies a particle from the vector into a GeantParticle
 */
bool GeantImporter::get_GeantParticle(int pdg, GeantParticle& g4Particle)
{
    for (auto particle : this->particle_vector_)
    {
        if (particle.pdg() == pdg)
        {
            g4Particle = particle;
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Copies a physics table from the vector<GeantPhysicsTable> into a single
 * GeantPhysicsTable object given the needed specifications
 */
bool GeantImporter::get_GeantPhysicsTable(GeantProcessType   process_type,
                                          GeantTableType     table_type,
                                          GeantProcess       process,
                                          GeantModel         model,
                                          PDGNumber          pdg,
                                          GeantPhysicsTable& physics_table)
{
    for (auto table : this->physics_table_vector_)
    {
        if (table.process_type() == process_type
            && table.table_type() == table_type && table.process() == process
            && table.model() == model && table.pdg() == (ssize_type)pdg.get())
        {
            physics_table = table;
            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a ParticleDef type by providing a PDG number
 */
ParticleDef GeantImporter::particleDef(ssize_type pdg)
{
    ParticleDef   particle;
    GeantParticle g4particle;

    get_GeantParticle(pdg, g4particle);
    particle = particleDef(g4particle);

    return particle;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a ParticleDef type by providing a GeantParticle type
 */
ParticleDef GeantImporter::particleDef(GeantParticle& g4particle)
{
    ParticleDef particle;

    particle.mass   = g4particle.mass();
    particle.charge = g4particle.charge();

    if (g4particle.is_stable())
        particle.decay_constant = ParticleDef::stable_decay_constant();
    else
        particle.decay_constant = 1. / g4particle.lifetime();

    return particle;
}

//---------------------------------------------------------------------------//
/*!
 * Creates a vector<ParticleDef> with all GeantParticle objects
 */
std::vector<ParticleDef> GeantImporter::particleDef_vector()
{
    std::vector<ParticleDef> particleVec;
    ParticleDef              particle;

    for (auto gParticleDef : this->particle_vector_)
    {
        particle = this->particleDef(gParticleDef);
        particleVec.push_back(particle);
    }

    return particleVec;
}

//---------------------------------------------------------------------------//
/*!
 * Creates a shared_ptr<ParticleParams> with all GeantParticle objects
 */

std::shared_ptr<ParticleParams> GeantImporter::get_ParticleParams()
{
    ParticleParams::VecAnnotatedDefs defs;

    for (auto particle : this->particle_vector_)
    {
        defs.push_back(
            // ParticleMd
            {{particle.name(), PDGNumber{particle.pdg()}},
            // ParticleDef
             {particle.mass(),
              particle.charge(),
              (particle.is_stable() ? ParticleDef::stable_decay_constant()
                                    : 1. / particle.lifetime())}});
    }
    return std::make_shared<ParticleParams>(std::move(defs));
}

//---------------------------------------------------------------------------//
/*!
 * Creates a list of all the object names found in the ROOT file
 */
void GeantImporter::build_object_list(std::string const root_folder)
{
    TDirectory* directory
        = (TDirectory*)this->root_input_->Get(root_folder.c_str());

    // Getting the list of elements in the ROOT file
    TList* list = directory->GetListOfKeys();

    // Getting list iterator
    TIter iter(list->MakeIterator());

    // Cleaning vector
    this->object_list_.clear();

    // Looping over the objects found in the ROOT file
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

        this->object_list_.push_back(keyName);
    }
}

//---------------------------------------------------------------------------//
} // namespace celeritas
