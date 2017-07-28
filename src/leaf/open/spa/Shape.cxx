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


// --- LEAF Includes --- //
#include <leaf/open/spa/Shape.h>

// --- Boost Includes --- //
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace spa
{

////////////////////////////////////////////////////////////////////////////////
Shape::Shape(
    unsigned int const& objId,
    std::string const& name,
    std::string const& fips,
    double const& latitude,
    double const& longitude,
    std::string const& countyName,
    std::string const& stabbrFips )
    :
    Object(),
    m_objId( objId ),
    m_name( name ),
    m_fips( fips ),
    m_latitude( latitude ),
    m_longitude( longitude ),
    m_countyName( countyName ),
    m_stabbrFips( stabbrFips )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Shape::~Shape()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int const& Shape::GetObjId() const
{
    return m_objId;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Shape::GetName() const
{
    return m_name;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Shape::GetFips() const
{
    return m_fips;
}
////////////////////////////////////////////////////////////////////////////////
double const& Shape::GetLatitude() const
{
    return m_latitude;
}
////////////////////////////////////////////////////////////////////////////////
double const& Shape::GetLongitude() const
{
    return m_longitude;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Shape::GetCountyName() const
{
    return m_countyName;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Shape::GetStabbrFips() const
{
    return m_stabbrFips;
}
////////////////////////////////////////////////////////////////////////////////

} //end spa
} //end open
} //end leaf
