//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTableParser.hh
//---------------------------------------------------------------------------//

#pragma once

// C++
#include <iostream>
#include <vector>
#include <memory>

class TFile;
class G4VProcess;
class G4ParticleDefinition;
class G4PhysicsTable;

class GeantPhysicsTableParser
{
public:
    GeantPhysicsTableParser() = default;
    ~GeantPhysicsTableParser() = default;
    
    void createRootFile(std::string rootFilename);
    void addPhysicsTableTree(std::string physTableAscFile);
    void addPhysicsTableTree(G4VProcess           * process,
                             G4ParticleDefinition * particle);

private:
    std::unique_ptr<TFile> rootOutput_;
    std::string rootOutputFilename_;
    
    int tableSize_;
    int vectorType_;
    double edgeMin_;
    double edgeMax_;
    int numberOfNodes_;
    int size_;
    
    std::vector<double> binVector_;  // Stores energy info
    std::vector<double> dataVector_; // Stores XS/Eloss info
    
    void writeTree(std::string tree_name, G4PhysicsTable * table);
};

void replaceCharacters(std::string &aString,
                       std::string const search,
                       std::string const replace);
