/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


#pragma once

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>

#include <leaf/open/Object.h>

#include <leaf/open/soi/Common.h>
#include <leaf/open/soi/SoilsPtr.h>
#include <leaf/open/soi/MapUnit.h>

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS Soils : public Object
{
public:
    ///
    virtual ~Soils();

    ///
    static SoilsPtr Create(
        std::string const& areatypename,
        std::string const& areasymbol,
        bool const& populate = true );

    ///
    MapUnitPtr GetMapUnit(
        std::string const& mukey,
        bool const& mustExist = false ) const;

#ifndef SWIG
    ///
    MapUnits const& GetMapUnits() const;

    ///
    MapUnitsByMuacresRange GetMapUnitsByMuacresRange(
        unsigned int const& lowerBound = 0,
        unsigned int const& upperBound = UINT_MAX ) const;
#endif //SWIG

    ///
    bool const& HasMapUnits() const;

    ///
    void Populate();

protected:
    ///
    Soils();

    ///
    Soils(
        std::string const& areatypename,
        std::string const& areasymbol,
        bool const& populate = true );

private:
    ///
    friend SoilsPtr boost::make_shared< Soils >();
    friend SoilsPtr boost::make_shared< Soils,
        std::string const&,
        std::string const&,
        bool const& >(
        std::string const&,
        std::string const&,
        bool const& );

    ///
    boost::once_flag m_of1;
    void PopulateMapUnits();

    ///
    std::string m_areatypename;

    ///
    std::string m_areasymbol;

    ///
    bool m_hasMapUnits;

    ///
    MapUnits m_mapUnits;

};

} //end soi
} //end open
} //end leaf
