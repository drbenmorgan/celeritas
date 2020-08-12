//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file GeantPhysicsTable.cc
//---------------------------------------------------------------------------//
#include "GeantPhysicsTable.hh"
#include "GeantImporter.hh"

#include <iomanip>
#include <iostream>

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Constructor and destructor
 */
GeantPhysicsTable::GeantPhysicsTable()
    : process_type_(GeantProcessType::not_defined)
    , process_type_name_("no_name")
    , table_type_(GeantTableType::not_defined)
    , table_type_name_("no name")
    , process_(GeantProcess::not_defined)
    , process_name_("no name")
    , model_(GeantModel::not_defined)
    , model_name_("no name")
    , particle_name_("no name")
    , pdg_(0)
{
}

GeantPhysicsTable::~GeantPhysicsTable() = default;

//---------------------------------------------------------------------------//
/*!
 * Operator ()
 */
void GeantPhysicsTable::operator()(
    GeantProcessType                process_type,
    GeantTableType                  table_type,
    GeantProcess                    process,
    GeantModel                      model,
    std::string                     particle_name,
    ssize_type                      pdg,
    std::vector<GeantPhysicsVector> physics_vectors)
{
    this->process_type_      = process_type;
    this->process_type_name_ = get_process_type_name(process_type);
    this->table_type_        = table_type;
    this->table_type_name_   = get_table_type_name(table_type);
    this->process_           = process;
    this->process_name_      = get_process_name(process);
    this->model_             = model;
    this->model_name_        = get_model_name(model);
    this->particle_name_     = particle_name;
    this->pdg_               = pdg;
    this->physics_vectors_   = physics_vectors;
}

//---------------------------------------------------------------------------//
/*!
 * Getters for the class members
 */
GeantProcessType GeantPhysicsTable::process_type()
{
    return this->process_type_;
}

std::string GeantPhysicsTable::process_type_name()
{
    return this->process_type_name_;
}

GeantTableType GeantPhysicsTable::table_type()
{
    return this->table_type_;
}

std::string GeantPhysicsTable::table_type_name()
{
    return this->table_type_name_;
}

GeantProcess GeantPhysicsTable::process()
{
    return this->process_;
}

std::string GeantPhysicsTable::process_name()
{
    return this->process_name_;
}

GeantModel GeantPhysicsTable::model()
{
    return this->model_;
}

std::string GeantPhysicsTable::model_name()
{
    return this->model_name_;
}

std::string GeantPhysicsTable::particle_name()
{
    return this->particle_name_;
}

ssize_type GeantPhysicsTable::pdg()
{
    return this->pdg_;
}

std::vector<GeantPhysicsVector> GeantPhysicsTable::physics_vectors()
{
    return this->physics_vectors_;
}

GeantPhysicsVector GeantPhysicsTable::physics_vector(ssize_t index)
{
    return this->physics_vectors_.at(index);
}

//---------------------------------------------------------------------------//
/*!
 * Prints all the data of a given GeantPhysicsTable
 */

void GeantPhysicsTable::print()
{
    std::cout << std::setprecision(3);
    std::cout << std::fixed;
    std::cout << std::scientific;

    std::cout << "=================================" << std::endl;
    std::cout << "Process type: " << this->process_type_name() << std::endl;
    std::cout << "Table type  : " << this->table_type_name() << std::endl;
    std::cout << "Process     : " << this->process_name() << std::endl;
    std::cout << "Model       : " << this->model_name() << std::endl;
    std::cout << "Particle    : " << this->particle_name() << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << " | Table size: " << this->physics_vectors_.size();
    std::cout << std::endl;

    for (ssize_t i = 0; i < this->physics_vectors_.size(); i++)
    {
        std::cout << " |" << std::endl;
        std::cout << " | ---------------------------" << std::endl;
        std::cout << " | ENTRY " << i << std::endl;
        std::cout << " | ---------------------------" << std::endl;
        std::cout << " | vector_type: ";
        std::cout << this->physics_vectors_.at(i).vector_type << std::endl;
        std::cout << " | energy         xs_eloss " << std::endl;

        for (ssize_t j = 0; j < this->physics_vectors_.at(i).energy.size(); j++)
        {
            std::cout << " | | ";
            std::cout << std::left << std::setw(12);
            std::cout << std::setfill(' ');
            std::cout << this->physics_vectors_.at(i).energy.at(j);
            std::cout << " | ";
            std::cout << this->physics_vectors_.at(i).xs_eloss.at(j);
            std::cout << std::endl;
        }
    }
    std::cout << "=================================" << std::endl;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a GeantProcessType enum provided a string process_type_name
 */
GeantProcessType
GeantPhysicsTable::get_process_type(std::string process_type_name)
{
    if (process_type_name == "not_defined")
        return GeantProcessType::not_defined;
    else if (process_type_name == "transportation")
        return GeantProcessType::transportation;
    else if (process_type_name == "electromagnetic")
        return GeantProcessType::electromagnetic;
    else if (process_type_name == "hadronic")
        return GeantProcessType::hadronic;
    else if (process_type_name == "photolepton_hadron")
        return GeantProcessType::photolepton_hadron;
    else if (process_type_name == "decay")
        return GeantProcessType::decay;
    else if (process_type_name == "general")
        return GeantProcessType::general;
    else if (process_type_name == "parameterisation")
        return GeantProcessType::parameterisation;
    else if (process_type_name == "user_defined")
        return GeantProcessType::user_defined;
    else if (process_type_name == "parallel")
        return GeantProcessType::parallel;
    else if (process_type_name == "phonon")
        return GeantProcessType::phonon;
    else if (process_type_name == "ucn")
        return GeantProcessType::ucn;
    else
        return GeantProcessType::not_defined;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a std::string process_type_name given a GeantProcessType enum
 */
std::string
GeantPhysicsTable::get_process_type_name(GeantProcessType process_type)
{
    switch (process_type)
    {
        case GeantProcessType::not_defined:
            return "not_defined";
        case GeantProcessType::transportation:
            return "transportation";
        case GeantProcessType::electromagnetic:
            return "electromagnetic";
        case GeantProcessType::optical:
            return "optical";
        case GeantProcessType::hadronic:
            return "hadronic";
        case GeantProcessType::photolepton_hadron:
            return "photolepton_hadron";
        case GeantProcessType::decay:
            return "decay";
        case GeantProcessType::general:
            return "general";
        case GeantProcessType::parameterisation:
            return "parameterisation";
        case GeantProcessType::user_defined:
            return "user_defined";
        case GeantProcessType::parallel:
            return "parallel";
        case GeantProcessType::phonon:
            return "phonon";
        case GeantProcessType::ucn:
            return "ucn";
    }
    return "not_defined";
}

//---------------------------------------------------------------------------//
/*!
 * Returns a GeantTableType enum provided a string table_type_name
 */
GeantTableType GeantPhysicsTable::get_table_type(std::string table_type_name)
{
    if (table_type_name == "not_defined")
        return GeantTableType::not_defined;
    else if (table_type_name == "DEDX")
        return GeantTableType::DEDX;
    else if (table_type_name == "Ionisation")
        return GeantTableType::Ionisation;
    else if (table_type_name == "Range")
        return GeantTableType::Range;
    else if (table_type_name == "RangeSec")
        return GeantTableType::RangeSec;
    else if (table_type_name == "InverseRange")
        return GeantTableType::InverseRange;
    else if (table_type_name == "Lambda")
        return GeantTableType::Lambda;
    else if (table_type_name == "LambdaPrim")
        return GeantTableType::LambdaPrim;
    else if (table_type_name == "LambdaMod1")
        return GeantTableType::LambdaMod1;
    else if (table_type_name == "LambdaMod2")
        return GeantTableType::LambdaMod2;
    else if (table_type_name == "LambdaMod3")
        return GeantTableType::LambdaMod3;
    else if (table_type_name == "LambdaMod4")
        return GeantTableType::LambdaMod4;
    else
        return GeantTableType::not_defined;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a std::string table_type_name given a GeantTableType enum
 */
std::string GeantPhysicsTable::get_table_type_name(GeantTableType table_type)
{
    switch (table_type)
    {
        case GeantTableType::not_defined:
            return "not_defined";
        case GeantTableType::DEDX:
            return "DEDX";
        case GeantTableType::Ionisation:
            return "Ionisation";
        case GeantTableType::Range:
            return "Range";
        case GeantTableType::RangeSec:
            return "RangeSec";
        case GeantTableType::InverseRange:
            return "InverseRange";
        case GeantTableType::Lambda:
            return "Lambda";
        case GeantTableType::LambdaPrim:
            return "LambdaPrim";
        case GeantTableType::LambdaMod1:
            return "LambdaMod1";
        case GeantTableType::LambdaMod2:
            return "LambdaMod2";
        case GeantTableType::LambdaMod3:
            return "LambdaMod3";
        case GeantTableType::LambdaMod4:
            return "LambdaMod4";
    }
    return "not_defined";
}

//---------------------------------------------------------------------------//
/*!
 * Returns a GeantProcess enum provided a string process_name
 */
GeantProcess GeantPhysicsTable::get_process(std::string process_name)
{
    if (process_name == "not_defined")
        return GeantProcess::not_defined;
    else if (process_name == "ionIoni")
        return GeantProcess::ionIoni;
    else if (process_name == "msc")
        return GeantProcess::msc;
    else if (process_name == "hIoni")
        return GeantProcess::hIoni;
    else if (process_name == "hBrems")
        return GeantProcess::hBrems;
    else if (process_name == "hPairProd")
        return GeantProcess::hPairProd;
    else if (process_name == "CoulombScat")
        return GeantProcess::CoulombScat;
    else if (process_name == "eIoni")
        return GeantProcess::eIoni;
    else if (process_name == "eBrem")
        return GeantProcess::eBrem;
    else if (process_name == "phot")
        return GeantProcess::phot;
    else if (process_name == "compt")
        return GeantProcess::compt;
    else if (process_name == "conv")
        return GeantProcess::conv;
    else if (process_name == "Rayl")
        return GeantProcess::Rayl;
    else if (process_name == "muIoni")
        return GeantProcess::muIoni;
    else if (process_name == "muBrems")
        return GeantProcess::muBrems;
    else if (process_name == "muPairProd")
        return GeantProcess::muPairProd;
    else
        return GeantProcess::not_defined;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a std::string process_name given a GeantProcess enum
 */
std::string GeantPhysicsTable::get_process_name(GeantProcess process)
{
    switch (process)
    {
        case GeantProcess::not_defined:
            return "not_defined";
        case GeantProcess::ionIoni:
            return "ionIoni";
        case GeantProcess::msc:
            return "msc";
        case GeantProcess::hIoni:
            return "hIoni";
        case GeantProcess::hBrems:
            return "hBrems";
        case GeantProcess::hPairProd:
            return "hPairProd";
        case GeantProcess::CoulombScat:
            return "CoulombScat";
        case GeantProcess::eIoni:
            return "eIoni";
        case GeantProcess::eBrem:
            return "eBrem";
        case GeantProcess::phot:
            return "phot";
        case GeantProcess::compt:
            return "compt";
        case GeantProcess::conv:
            return "conv";
        case GeantProcess::Rayl:
            return "Rayl";
        case GeantProcess::muIoni:
            return "muIoni";
        case GeantProcess::muBrems:
            return "muBrems";
        case GeantProcess::muPairProd:
            return "muPairProd";
    }
    return "not_defined";
}

//---------------------------------------------------------------------------//
/*!
 * Returns a GeantModel enum provided a string model_name
 */
GeantModel GeantPhysicsTable::get_model(std::string model_name)
{
    if (model_name == "not_defined")
        return GeantModel::not_defined;
    else if (model_name == "BraggIon")
        return GeantModel::BraggIon;
    else if (model_name == "BetheBloch")
        return GeantModel::BetheBloch;
    else if (model_name == "UrbanMsc")
        return GeantModel::UrbanMsc;
    else if (model_name == "ICRU73QO")
        return GeantModel::ICRU73QO;
    else if (model_name == "WentzelVIUni")
        return GeantModel::WentzelVIUni;
    else if (model_name == "hBrem")
        return GeantModel::hBrem;
    else if (model_name == "hPairProd")
        return GeantModel::hPairProd;
    else if (model_name == "eCoulombScattering")
        return GeantModel::eCoulombScattering;
    else if (model_name == "Bragg")
        return GeantModel::Bragg;
    else if (model_name == "MollerBhabha")
        return GeantModel::MollerBhabha;
    else if (model_name == "eBremSB")
        return GeantModel::eBremSB;
    else if (model_name == "eBremLPM")
        return GeantModel::eBremLPM;
    else if (model_name == "LivermorePhElectric")
        return GeantModel::LivermorePhElectric;
    else if (model_name == "Klein-Nishina")
        return GeantModel::KleinNishina;
    else if (model_name == "BetheHeitlerLPM")
        return GeantModel::BetheHeitlerLPM;
    else if (model_name == "LivermoreRayleigh")
        return GeantModel::LivermoreRayleigh;
    else if (model_name == "MuBetheBloch")
        return GeantModel::MuBetheBloch;
    else if (model_name == "MuBrem")
        return GeantModel::MuBrem;
    else if (model_name == "muPairProd")
        return GeantModel::muPairProd;
    else
        return GeantModel::not_defined;
}

//---------------------------------------------------------------------------//
/*!
 * Returns a std::string model_name given a GeantModel enum
 */
std::string GeantPhysicsTable::get_model_name(GeantModel model)
{
    switch (model)
    {
        case GeantModel::not_defined:
            return "not_defined";
        case GeantModel::BraggIon:
            return "BraggIon";
        case GeantModel::BetheBloch:
            return "BetheBloch";
        case GeantModel::UrbanMsc:
            return "UrbanMsc";
        case GeantModel::ICRU73QO:
            return "ICRU73QO";
        case GeantModel::WentzelVIUni:
            return "WentzelVIUni";
        case GeantModel::hBrem:
            return "hBrem";
        case GeantModel::hPairProd:
            return "hPairProd";
        case GeantModel::eCoulombScattering:
            return "eCoulombScattering";
        case GeantModel::Bragg:
            return "Bragg";
        case GeantModel::MollerBhabha:
            return "MollerBhabha";
        case GeantModel::eBremSB:
            return "eBremSB";
        case GeantModel::eBremLPM:
            return "eBremLPM";
        case GeantModel::LivermorePhElectric:
            return "LivermorePhElectric";
        case GeantModel::KleinNishina:
            return "Klein-Nishina";
        case GeantModel::BetheHeitlerLPM:
            return "BetheHeitlerLPM";
        case GeantModel::LivermoreRayleigh:
            return "LivermoreRayleigh";
        case GeantModel::MuBetheBloch:
            return "MuBetheBloch";
        case GeantModel::MuBrem:
            return "MuBrem";
        case GeantModel::muPairProd:
            return "muPairProd";
    }
    return "not_defined";
}

//---------------------------------------------------------------------------//
} // namespace celeritas
