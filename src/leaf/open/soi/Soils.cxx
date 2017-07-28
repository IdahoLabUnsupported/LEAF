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
#include <leaf/open/soi/Soils.h>

// --- Poco Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace soi
{

////////////////////////////////////////////////////////////////////////////////
Soils::Soils(
    std::string const& areatypename,
    std::string const& areasymbol,
    bool const& populate )
    :
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_areatypename( areatypename ),
    m_areasymbol( areasymbol ),
    m_hasMapUnits( false ),
    m_mapUnits()
{
    if( populate ) Populate();
}
////////////////////////////////////////////////////////////////////////////////
Soils::~Soils()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SoilsPtr Soils::Create(
    std::string const& areatypename,
    std::string const& areasymbol,
    bool const& populate )
{
    return boost::make_shared< Soils >( areatypename, areasymbol, populate );
}
////////////////////////////////////////////////////////////////////////////////
MapUnitPtr Soils::GetMapUnit(
    std::string const& mukey,
    bool const& mustExist ) const
{
    MapUnitsByMukey::const_iterator itr =
        m_mapUnits.get< MUKEY >().find( mukey );
    if( itr == m_mapUnits.get< MUKEY >().end() )
    {
        if( !mustExist ) return MapUnitPtr();

        std::stringstream err;
        err << "leaf::open::soi::Soils::GetMapUnit( \"" << mukey
            << "\" ) could not find specified mukey in MapUnit map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
MapUnits const& Soils::GetMapUnits() const
{
    return m_mapUnits;
}
////////////////////////////////////////////////////////////////////////////////
MapUnitsByMuacresRange Soils::GetMapUnitsByMuacresRange(
    unsigned int const& lowerBound,
    unsigned int const& upperBound ) const
{
    return m_mapUnits.get< MUACRES >().range(
        boost::bind< bool >( util::less_than_or_equal_to< long >(),
            boost::lambda::_1, upperBound ),
        boost::bind< bool >( util::greater_than_or_equal_to< long >(),
            boost::lambda::_1, lowerBound ) );
}
////////////////////////////////////////////////////////////////////////////////
bool const& Soils::HasMapUnits() const
{
    return m_hasMapUnits;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Populate()
{
    boost::call_once( m_of1, boost::bind( &Soils::PopulateMapUnits, this ) );
}
////////////////////////////////////////////////////////////////////////////////
void Soils::PopulateMapUnits()
{
    ( GetSession( DB_SSURGO )
        << MapUnit::INPUT << "\n"
        << "  inner join laoverlap\n"
        << "    on laoverlap.lkey = legend.lkey and\n"
        << "       laoverlap.areatypename = ? and\n"
        << "       laoverlap.areasymbol = ?",
        into( m_mapUnits ),
        useRef( m_areatypename ),
        useRef( m_areasymbol ),
        now );

    m_hasMapUnits = ( m_mapUnits.size() != 0 );

    MapUnitsByMukey::const_iterator itr =
        m_mapUnits.get< MUKEY >().begin();
    for( ; itr != m_mapUnits.get< MUKEY >().end(); ++itr )
    {
        (*itr)->Populate();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
