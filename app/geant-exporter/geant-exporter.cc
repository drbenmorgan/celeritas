//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file geant-exporter.cc
//---------------------------------------------------------------------------//
/*! \brief Geant4 particle definition and physics tables exporter
 *
 *  The code exports particle information and physics tables built by the
 *  selected physics list and geometry. Output data is stored into ROOT files
 *  and read by Celeritas' Geant Importer class.
 */
//---------------------------------------------------------------------------//

// C++
#include <string>
#include <vector>

// Project
#include "src/DetectorConstruction.hh"
#include "src/ActionInitialization.hh"
#include "src/GeantPhysicsTableParser.hh"

// Geant4
#include "G4RunManager.hh"
#include "G4UImanager.hh"
//#include "FTFP_BERT.hh"
#include "G4VModularPhysicsList.hh"
#include "G4GenericPhysicsList.hh"
#include "G4PhysicsTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"

// ROOT
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TBranch.h"

int main(int argc, char** argv)
{
    //-----------------------------------------------------------------------//
    //! Minimal help
    if (argc == 1 || argc != 2)
    {
        G4cout << "Usage: ./geant4-exporter geometry.gdml" << G4endl;
        return 0;
    }

    //-----------------------------------------------------------------------//
    //! Starting the run manager
    auto run_manager = new G4RunManager;

    //-----------------------------------------------------------------------//
    //! Fetching the GDML input file and initializing the geometry
    std::string gdml_input = argv[argc - 1];

    run_manager->SetUserInitialization(new DetectorConstruction(gdml_input));

    //-----------------------------------------------------------------------//
    //! Loading the physics list
    std::vector<G4String>* physics_constructor = new std::vector<G4String>;
    physics_constructor->push_back("G4EmStandardPhysics");

    G4VModularPhysicsList* physics_list;

    // Adding the user-defined physics list
    physics_list = new G4GenericPhysicsList(physics_constructor);

    // For the full Physics List:
    // Uncomment #include "FTFP_BERT.hh" and use the following pointer instead
    // physicsList = new FTFP_BERT;

    // Initializing the Physics List
    run_manager->SetUserInitialization(physics_list);

    //-----------------------------------------------------------------------//
    //! The ActionInitialization class is responsible for calling all the
    //! remaining geant classes
    run_manager->SetUserInitialization(new ActionInitialization());

    //-----------------------------------------------------------------------//
    //! Starting a minimal run to produce the physics tables
    auto ui_manager = G4UImanager::GetUIpointer();
    ui_manager->ApplyCommand("/run/initialize");

    run_manager->BeamOn(1);

    //-----------------------------------------------------------------------//
    /*!
     *  Storing both particle definition information and physics tables
     *
     *  Code was splitted into two separate loops for simplicity:
     *  - First loop stores particle info; second stores physics tables
     *  - Minimal runtime difference penalty in favor of an easier readability
     */
    //-----------------------------------------------------------------------//

    auto particle_iterator = G4ParticleTable::GetParticleTable()->GetIterator();

    //-----------------------------------------------------------------------//
    //! LOOP 1: Storing particle information
    //-----------------------------------------------------------------------//

    // Creating the particle definition root file
    G4String               root_output_filename = "geant-exporter-data.root";
    std::unique_ptr<TFile> root_output(
        new TFile(root_output_filename.c_str(), "recreate"));

    TDirectory* particles = root_output->mkdir("particles");
    particles->cd();

    std::cout << "Creating " << root_output_filename << "..." << std::endl;

    // Creating variables to reference the root branches
    std::string name;
    int         pdg;
    double      mass, charge, lifetime, spin;
    bool        is_stable;

    particle_iterator->reset();

    while ((*particle_iterator)())
    {
        G4ParticleDefinition* particle = particle_iterator->value();

        name      = particle->GetParticleName();
        pdg       = particle->GetPDGEncoding();
        mass      = particle->GetPDGMass();
        charge    = particle->GetPDGCharge();
        lifetime  = particle->GetPDGLifeTime();
        spin      = particle->GetPDGSpin();
        is_stable = particle->GetPDGStable();

        // Converting lifetime of unstable particles to seconds
        // For stable particles lifetime = -1
        if (!is_stable)
        {
            lifetime *= 1E-9;
        }

        // ROOT does not like special characters for its TTree names
        replace_characters(name, "+", "Plus");
        replace_characters(name, "-", "Minus");

        // Gamma is a ROOT function. A TTree with that same name confuses ROOT
        if (name == "gamma")
        {
            name = "photon";
        }

        // Creating a TTree for each particle
        TTree* tree_particle = new TTree(name.c_str(), name.c_str());

        // Preserving the original particle name in the data
        name = particle->GetParticleName();

        tree_particle->Branch("name", &name);
        tree_particle->Branch("pdg", &pdg, "pdg/I");
        tree_particle->Branch("mass", &mass, "mass/D");
        tree_particle->Branch("charge", &charge, "charge/D");
        tree_particle->Branch("spin", &spin, "spin/D");
        tree_particle->Branch("lifetime", &lifetime, "lifetime/D");
        tree_particle->Branch("is_stable", &is_stable, "is_stable/O");

        tree_particle->Fill();

        std::cout << "  Added " << name << std::endl;
    }

    root_output->Write();
    root_output->Close();
    std::cout << root_output_filename << " done!" << std::endl;
    std::cout << std::endl;

    //-----------------------------------------------------------------------//
    //! LOOP 2: Storing physics tables into a ROOT file
    //-----------------------------------------------------------------------//

    GeantPhysicsTableParser table_parser;
    table_parser.root_file_name(root_output_filename);

    particle_iterator->reset();

    while ((*particle_iterator)())
    {
        // Fetching particle and its data
        G4ParticleDefinition* particle = particle_iterator->value();

        std::cout << "=============" << std::endl;
        std::cout << particle->GetParticleName() << std::endl;
        std::cout << "=============" << std::endl;

        G4ProcessVector* process_list
            = (particle->GetProcessManager())->GetProcessList();

        // Looping over processes
        for (std::size_t j = 0; j < process_list->size(); j++)
        {
            G4VProcess* process = (*process_list)[j];
            table_parser.add_physics_table(process, particle);
        }
    }

    std::cout << root_output_filename << " done!" << std::endl;

    //---------------------------- Job termination --------------------------//
    /*! User actions, physics list, and detector construction are owned and
     *  deleted by the run manager
     */
    delete run_manager;
}