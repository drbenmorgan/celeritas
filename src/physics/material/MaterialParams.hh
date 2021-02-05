//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file MaterialParams.hh
//---------------------------------------------------------------------------//
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "base/DeviceVector.hh"
#include "base/Types.hh"
#include "physics/base/Units.hh"
#include "MaterialInterface.hh"
#include "Types.hh"
#include "MaterialInterface.hh"

namespace celeritas
{
//---------------------------------------------------------------------------//
/*!
 * Data management for material, element, and nuclide properties.
 */
class MaterialParams
{
  public:
    //!@{
    //! References to constsructed data
    using HostRef
        = MaterialParamsData<Ownership::const_reference, MemSpace::host>;
    using DeviceRef
        = MaterialParamsData<Ownership::const_reference, MemSpace::device>;
    using size_type = pie_size_type;
    //!@}

    //! Define an element's input data
    struct ElementInput
    {
        int            atomic_number; //!< Z number
        units::AmuMass atomic_mass;   //!< Isotope-weighted average atomic mass
        std::string    name;          //!< Element name
    };

    //! Define a material's input data
    struct MaterialInput
    {
        real_type   number_density; //!< Atomic number density [1/cm^3]
        real_type   temperature;    //!< Temperature [K]
        MatterState matter_state;   //!< Solid, liquid, gas
        std::vector<std::pair<ElementId, real_type>>
                    elements_fractions; //!< Fraction of number density
        std::string name;               //!< Material name
    };

    //! Input data to construct this class
    struct Input
    {
        std::vector<ElementInput>  elements;
        std::vector<MaterialInput> materials;
    };

  public:
    // Construct with a vector of material definitions
    explicit MaterialParams(const Input& inp);

    // Get element name
    inline const std::string& id_to_label(ElementId id) const;

    // Get material name
    inline const std::string& id_to_label(MaterialId id) const;

    // Find a material from a name
    // TODO: Map different MaterialDefIds with same material name
    inline MaterialId find(const std::string& name) const;

    // Access material properties on the host
    inline const HostRef& host_pointers() const;

    // Access material properties on the device
    inline const DeviceRef& device_pointers() const;

    //! Maximum number of elements in any one material
    size_type max_element_components() const { return max_el_; }

  private:
    std::vector<std::string>                    elnames_;
    std::vector<std::string>                    matnames_;
    std::unordered_map<std::string, MaterialId> matname_to_id_;
    size_type                                   max_el_;

    // Host/device sstorage and reference
    CELER_PIE_STRUCT(MaterialParamsData, const_reference) data_;

    // HELPER FUNCTIONS
    void                      append_element_def(const ElementInput& inp);
    PieSlice<MatElementComponent> extend_elcomponents(const MaterialInput& inp);
    void                      append_material_def(const MaterialInput& inp);
};

//---------------------------------------------------------------------------//
} // namespace celeritas

#include "MaterialParams.i.hh"
