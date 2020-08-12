//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTable.hh
//---------------------------------------------------------------------------//
#pragma once

#include <vector>
#include <string>

#include "GeantPhysicsVector.hh"
#include "GeantProcessType.hh"
#include "GeantTableType.hh"
#include "GeantProcess.hh"
#include "GeantModel.hh"
#include "base/Types.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * /brief Stores physics table information
 *
 * This class provides the same structure seen in the geant-exporter.root
 * file created by the app/geant-exporter external code. The GeantImporter
 * class is responsible for loading the data from the ROOT file into memory.
 */
class GeantPhysicsTable
{
  private:
    GeantProcessType                process_type_;
    std::string                     process_type_name_;
    GeantTableType                  table_type_;
    std::string                     table_type_name_;
    GeantProcess                    process_;
    std::string                     process_name_;
    GeantModel                      model_;
    std::string                     model_name_;
    std::string                     particle_name_;
    ssize_type                      pdg_;
    std::vector<GeantPhysicsVector> physics_vectors_;

  public:
    GeantPhysicsTable();
    ~GeantPhysicsTable();

    void operator()(GeantProcessType                process_type,
                    GeantTableType                  table_type,
                    GeantProcess                    process,
                    GeantModel                      model,
                    std::string                     particle_name,
                    ssize_type                      pdg,
                    std::vector<GeantPhysicsVector> physics_vectors);

    GeantProcessType                process_type();
    std::string                     process_type_name();
    GeantTableType                  table_type();
    std::string                     table_type_name();
    GeantProcess                    process();
    std::string                     process_name();
    GeantModel                      model();
    std::string                     model_name();
    std::string                     particle_name();
    ssize_type                      pdg();
    std::vector<GeantPhysicsVector> physics_vectors();

    GeantPhysicsVector physics_vector(ssize_t index);

    void print();

    GeantProcessType get_process_type(std::string process_type_name);
    std::string      get_process_type_name(GeantProcessType process_type);

    GeantTableType get_table_type(std::string table_type_name);
    std::string    get_table_type_name(GeantTableType table_type);

    GeantProcess get_process(std::string process_name);
    std::string  get_process_name(GeantProcess process);

    GeantModel  get_model(std::string model_name);
    std::string get_model_name(GeantModel model);
};

//---------------------------------------------------------------------------//
} // namespace celeritas
