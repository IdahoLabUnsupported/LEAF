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

#include <leaf/open/spa/ShapePtr.h>

#include <leaf/util/DataHelper.h>

//No namespace for these
struct OBJID{};

namespace leaf
{
namespace open
{
namespace spa
{

class LEAF_OPEN_EXPORTS Shape : public Object
{
public:
    ///
    enum Enum
    {
        POINT,
        POLYLINE,
        POLYGON
    };

    ///Constructor
    Shape(
        unsigned int const& objId,
        std::string const& name,
        std::string const& fips,
        double const& latitude,
        double const& longitude,
        std::string const& countyName,
        std::string const& stabbrFips );

    ///Destructor
    virtual ~Shape();

    ///
    unsigned int const& GetObjId() const;

    ///
    std::string const& GetName() const;

    ///
    std::string const& GetFips() const;

    ///
    double const& GetLatitude() const;

    ///
    double const& GetLongitude() const;

    ///
    std::string const& GetCountyName() const;

    ///
    std::string const& GetStabbrFips() const;

protected:

private:
    ///
    unsigned int m_objId;

    ///
    std::string m_name;

    ///
    std::string m_fips;

    ///
    double m_latitude;

    ///
    double m_longitude;

    ///
    std::string m_countyName;

    ///
    std::string m_stabbrFips;

};

///Define a multiply indexed set with indices by mukey
typedef boost::multi_index_container<
    ShapePtr,
    boost::multi_index::indexed_by<
        //Sort by mukey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< OBJID >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN(
                Shape, unsigned int const&, GetObjId )
        >
    >
> Shapes;

///
typedef Shapes::index< OBJID >::type ShapesByObjId;

} //end spa
} //end open
} //end leaf
