//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTableParser.cc
//---------------------------------------------------------------------------//


#include "GeantPhysicsTableParser.hh"

// C++
#include <fstream>

// Root
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

// Geant4
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"
#include "G4VEmProcess.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4VMultipleScattering.hh"
#include "G4eplusAnnihilation.hh"

#include "G4PhysicsTable.hh"
#include "G4PhysicsVector.hh"


//---------------------------------------------------------------------------//
//! Creates a new ROOT file with a given filename
void GeantPhysicsTableParser::createRootFile(std::string rootFilename)
{
    this->rootOutputFilename_ = rootFilename;
    this->rootOutput_ = std::make_unique<TFile>(rootFilename.c_str(), "recreate");
    
    rootOutput_->Write();
    rootOutput_->Close();
    
    std::cout << "Creating " << rootFilename << "..." << std::endl;
}


//---------------------------------------------------------------------------//
//! Adds a new physics table as a new TTree by providing the asc file
void GeantPhysicsTableParser::addPhysicsTableTree(std::string physTableAscFile)
{
    // Opening input
    std::ifstream inputFile;
    inputFile.open(physTableAscFile, std::ios::in);
    
    // Updating the physTableAscFile string to become the TTree name
    // Removing .asc at the end
    physTableAscFile.erase(physTableAscFile.length() - 4);
    
    // Removing the directory path, if any
    std::size_t lastPathSlash = physTableAscFile.find_last_of("/");
    physTableAscFile = physTableAscFile.substr(lastPathSlash + 1);
    
    // Replacing special characters. ROOT TTrees do not bode well with them
    replaceCharacters(physTableAscFile, ".", "_");
    replaceCharacters(physTableAscFile, "+", "Plus");
    replaceCharacters(physTableAscFile, "-", "Minus");
    
    // Updating root file
    rootOutput_ = std::make_unique<TFile>(rootOutputFilename_.c_str(), "update");
    
    // Creating the TTree
    TTree *tree = new TTree(physTableAscFile.c_str(),
                            physTableAscFile.c_str());
    
    tree->Branch("edgeMin", &edgeMin_, "edgeMin/D");
    tree->Branch("edgeMax", &edgeMax_, "edgeMax/D");
    tree->Branch("numberOfNodes", &numberOfNodes_, "numberOfNodes/I");
    tree->Branch("vectorType", &vectorType_, "vectorType/I");
    tree->Branch("binVector", &binVector_);
    tree->Branch("dataVector", &dataVector_);
    
    // Reading file
    inputFile >> this->tableSize_;
    
    for (int i = 0; i < this->tableSize_; i++)
    {
        binVector_.clear();
        dataVector_.clear();
        
        inputFile >> this->vectorType_;
        inputFile >> this->edgeMin_ >> this->edgeMax_ >> this->numberOfNodes_;
        inputFile >> this->size_;
        
        if (this->size_ == 0)
        {
            std::cout << "Size 0" << std::endl;
            this->rootOutput_->Close();
            inputFile.close();
            return;
        }
        
        double bin, data;
        for (int j = 0; j < size_; j++)
        {
            inputFile >> bin >> data;
            
            this->binVector_.push_back(bin);
            this->dataVector_.push_back(data);
        }
        
        tree->Fill();
    }
    
    // Writing and closing files
    this->rootOutput_->Write();
    this->rootOutput_->Close();
    inputFile.close();
    
    std::cout << "  Added " << physTableAscFile << std::endl;
}


//---------------------------------------------------------------------------//
//! Adds a new physics table as a new TTree by providing process and particle
void GeantPhysicsTableParser::addPhysicsTableTree(G4VProcess          *process,
                                                  G4ParticleDefinition*particle)
{
    G4ProcessType process_type = process->GetProcessType();
    G4String process_type_name = process->GetProcessTypeName(process_type);

    // Keeping only EM processes/models
    if (process_type_name == "Transportation") { return; }
    
    std::string tree_name;
    G4String particle_name = particle->GetParticleName();
    G4String process_name = process->GetProcessName();
    
    auto em_process = dynamic_cast<G4VEmProcess*>(process);
    auto eloss_process = dynamic_cast<G4VEnergyLossProcess*>(process);
    auto multiple_scattering = dynamic_cast<G4VMultipleScattering*>(process);
    
    //-----------------------------------------------------------------------//
    //! G4VEmProcess
    if (em_process)
    {
        G4PhysicsTable * table = nullptr;
        
        if (em_process->LambdaTable())
        {
            table = em_process->LambdaTable();
            tree_name = "Lambda_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (em_process->LambdaTablePrim())
        {
            table = em_process->LambdaTablePrim();
            tree_name = "LambdaPrim_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
                
        if (table == nullptr)
        {
            std::cout << "No available table in ";
            std::cout << "GeantPhysicsTableParser::addPhysicsTableTree for ";
            std::cout << "G4VEmProcess " << process_name << ".";
            std::cout << particle_name << std::endl;
        }
    }
    
    //-----------------------------------------------------------------------//
    //! G4VEnergyLossProcess
    else if (eloss_process)
    {
        G4PhysicsTable * table = nullptr;
             
        if (eloss_process->DEDXTable())
        {
            table = eloss_process->DEDXTable();
            tree_name = "DEDX_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->DEDXTableForSubsec())
        {
            table = eloss_process->DEDXTableForSubsec();
            tree_name = "SubDEDX_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->DEDXunRestrictedTable())
        {
            table = eloss_process->DEDXunRestrictedTable();
            tree_name = "DEDXnr_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->IonisationTable())
        {
            table = eloss_process->IonisationTable();
            tree_name = "Ionisation_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->IonisationTableForSubsec())
        {
            table = eloss_process->IonisationTableForSubsec();
            tree_name = "SubIonisation_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->CSDARangeTable())
        {
            table = eloss_process->CSDARangeTable();
            tree_name = "CSDARange_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->SecondaryRangeTable())
        {
            table = eloss_process->SecondaryRangeTable();
            tree_name = "RangeSec_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->RangeTableForLoss())
        {
            table = eloss_process->RangeTableForLoss();
            tree_name = "Range_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->InverseRangeTable())
        {
            table = eloss_process->InverseRangeTable();
            tree_name = "InverseRange_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->LambdaTable())
        {
            table = eloss_process->LambdaTable();
            tree_name = "Lambda_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (eloss_process->SubLambdaTable())
        {
            table = eloss_process->SubLambdaTable();
            tree_name = "SubLambda_" + process_name + "_" + particle_name;
            writeTree(tree_name, table);
        }
        
        if (table == nullptr)
        {
            std::cout << "No available table in ";
            std::cout << "GeantPhysicsTableParser::addPhysicsTableTree for ";
            std::cout << "G4VEnergyLossProcess " << process_name << ".";
            std::cout << particle_name << std::endl;
        }
    }
    
    //-----------------------------------------------------------------------//
    //! G4VMultipleScattering
    else if (multiple_scattering)
    {
        for (int i = 0; i < 4; i++)
        {
            G4VEmModel * model = multiple_scattering->GetModelByIndex(i);
            
            if (model)
            {
                G4PhysicsTable * table = model->GetCrossSectionTable();
                if (table)
                {
                    G4String thisModelName = model->GetName();
                    tree_name = "LambdaMod" + std::to_string(i+1) + "_";
                    tree_name += process_name + "_" + particle_name;
                    writeTree(tree_name, table);
                }
            }
        }
    }
    
    //-----------------------------------------------------------------------//
    else
    {
        std::cout << "No available code for ";
        std::cout << particle_name << "." << process_name << std::endl;
    }
}


//---------------------------------------------------------------------------//
//! Writes a given physics table as a tree to this->rootOutput_
void GeantPhysicsTableParser::writeTree(std::string      tree_name,
                                        G4PhysicsTable * table)
{
    // Replacing special characters. ROOT TTrees do not bode well with them
    replaceCharacters(tree_name, ".", "_");
    replaceCharacters(tree_name, "+", "Plus");
    replaceCharacters(tree_name, "-", "Minus");
    
    rootOutput_ = std::make_unique<TFile>(rootOutputFilename_.c_str(),"update");
    
    // Creating the TTree
    TTree *tree = new TTree(tree_name.c_str(),
                            tree_name.c_str());
    
    tree->Branch("edgeMin", &edgeMin_, "edgeMin/D");
    tree->Branch("edgeMax", &edgeMax_, "edgeMax/D");
    tree->Branch("numberOfNodes", &numberOfNodes_, "numberOfNodes/I");
    tree->Branch("vectorType", &vectorType_, "vectorType/I");
    tree->Branch("binVector", &binVector_);
    tree->Branch("dataVector", &dataVector_);
    
    this->tableSize_ = table->size();
    
    // G4PhysicsTable is a std::vector<G4PhysicsVector>
    for (auto phys_vector : *table)
    {
        this->edgeMin_ = phys_vector->Energy(0);
        this->edgeMax_ = phys_vector->GetMaxEnergy();
        this->vectorType_ = phys_vector->GetType();
        this->numberOfNodes_ = phys_vector->GetVectorLength();
        this->binVector_.clear();
        this->dataVector_.clear();
        
        for (std::size_t j = 0; j < this->numberOfNodes_; j++)
        {
            this->binVector_.push_back(phys_vector->Energy(j));
            this->dataVector_.push_back((*phys_vector)[j]);
        }
        
        tree->Fill();
    }
    
    this->rootOutput_->Write();
    this->rootOutput_->Close();
    
    std::cout << "  Added " << tree_name << std::endl;
}


//---------------------------------------------------------------------------//
//! Replaces characters in a string
void replaceCharacters(std::string &aString,
                       std::string const search,
                       std::string const replace)
{
    size_t position = 0;
    
    while ((position = aString.find(search, position)) != std::string::npos)
    {
        aString.replace(position, search.length(), replace);
        position += replace.length();
    }
}
