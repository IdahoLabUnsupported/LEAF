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
#include <leaf/open/soi/MapUnit.h>

using namespace leaf::util;

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
std::string const MapUnit::INPUT =
    "select\n"
    "  legend.areaname,\n"
    "  legend.areasymbol,\n"
    "  muaggatt.brockdepmin,\n"
    "  mapunit.lkey,\n"
    "  mapunit.muacres,\n"
    "  mapunit.mukey,\n"
    "  mapunit.muname,\n"
    "  mapunit.musym\n"
    "from legend\n"
    "  inner join mapunit\n"
    "    on mapunit.lkey = legend.lkey and\n"
    "       legend.areatypename = 'Non-MLRA Soil Survey Area'\n"
    "  left outer join muaggatt\n"
    "    on muaggatt.mukey = mapunit.mukey";
////////////////////////////////////////////////////////////////////////////////
MapUnit::MapUnit()
    :
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_areaname_legend(),
    m_areasymbol_legend(),
    m_brockdepmin(),
    m_lkey(),
    m_muacres(),
    m_mukey(),
    m_muname(),
    m_musym(),
    m_hasComponents( false ),
    m_components()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
MapUnit::MapUnit(
    std::string const& key,
    bool const& populate )
    :
    //A delegating constructor would be nice here: C++0x functionality
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_areaname_legend(),
    m_areasymbol_legend(),
    m_brockdepmin(),
    m_lkey(),
    m_muacres(),
    m_mukey(),
    m_muname(),
    m_musym(),
    m_hasComponents( false ),
    m_components()
{
    std::string mukey = key.substr( 0, key.find( ":" ) );
    ( GetSession( DB_SSURGO )
        << MapUnit::INPUT << "\n"
        << "where\n"
        << "  mapunit.mukey = ?",
        into( *this ),
        useRef( mukey ),
        now );

    if( populate ) Populate();
}
////////////////////////////////////////////////////////////////////////////////
MapUnit::MapUnit(
    std::string const& areatypename,
    std::string const& areasymbol,
    std::string const& musym,
    bool const& populate )
    :
    //A delegating constructor would be nice here: C++0x functionality
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_areaname_legend(),
    m_areasymbol_legend(),
    m_brockdepmin(),
    m_lkey(),
    m_muacres(),
    m_mukey(),
    m_muname(),
    m_musym(),
    m_hasComponents( false ),
    m_components()
{
    ( GetSession( DB_SSURGO )
        << MapUnit::INPUT << "\n"
        << "  inner join laoverlap\n"
        << "    on laoverlap.lkey = legend.lkey and\n"
        << "       laoverlap.areatypename = ? and\n"
        << "       laoverlap.areasymbol = ?\n"
        << "where\n"
        << "  mapunit.musym = ?",
        into( *this ),
        useRef( areatypename ),
        useRef( areasymbol ),
        useRef( musym ),
        now );

    if( populate ) Populate();
}
////////////////////////////////////////////////////////////////////////////////
MapUnit::~MapUnit()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
double MapUnit::AcresIn(
    std::string const& areatypename,
    std::string const& areasymbol ) const
{
    //A mapunit only has one [lkey];
    //For laoverlap, each [lkey, areatypename, areasymbol] combination is unique
    //Therefore the following query is guaranteed to return a single result
    double acresIn( 0.0 );
    ( GetSession( DB_SSURGO )
        << "select\n"
        << "  muaoverlap.areaovacres\n"
        << "from mapunit\n"
        << "  inner join muaoverlap\n"
        << "    on muaoverlap.mukey = mapunit.mukey\n"
        << "  inner join laoverlap\n"
        << "    on laoverlap.lareaovkey = muaoverlap.lareaovkey and\n"
        << "       laoverlap.areatypename = ? and\n"
        << "       laoverlap.areasymbol = ?\n"
        << "where\n"
        << "  mapunit.mukey = ?",
        into( acresIn ),
        useRef( areatypename ),
        useRef( areasymbol ),
        useRef( m_mukey ),
        now );
    assert( acresIn >= 0.0 );

    return acresIn;
}
////////////////////////////////////////////////////////////////////////////////
MapUnitPtr MapUnit::Create(
    std::string const& key,
    bool const& populate )
{
    return boost::make_shared< MapUnit >( key, populate );
}
////////////////////////////////////////////////////////////////////////////////
MapUnitPtr MapUnit::Create(
    std::string const& areatypename,
    std::string const& areasymbol,
    std::string const& musym,
    bool const& populate )
{
    return boost::make_shared< MapUnit >(
        areatypename, areasymbol, musym, populate );
}
////////////////////////////////////////////////////////////////////////////////
ComponentPtr MapUnit::GetComponent(
    std::string const& cokey,
    bool const& mustExist ) const
{
    ComponentsByCokey::const_iterator itr =
        m_components.get< COKEY >().find( cokey );
    if( itr == m_components.get< COKEY >().end() )
    {
        if( !mustExist ) return ComponentPtr();

        std::stringstream err;
        err << "leaf::open::soi::MapUnit::GetComponentPtr( \"" << cokey
            << "\" ) could not find specified cokey in Component map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
ComponentPtr MapUnit::GetDominantComponent() const
{
    ComponentsByComppct::const_iterator itr =
        m_components.get< COMPPCT >().begin();
    if( itr == m_components.get< COMPPCT >().end() )
    {
        std::stringstream err;
        err << "leaf::open::soi::MapUnit::GetDominantComponent("
            << ") could not find dominant component!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
Components const& MapUnit::GetComponents() const
{
    return m_components;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& MapUnit::GetAreanameLegend() const
{
    return m_areaname_legend;
}
////////////////////////////////////////////////////////////////////////////////
Var const& MapUnit::GetAreasymbolLegend() const
{
    return m_areasymbol_legend;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& MapUnit::GetBrockdepmin() const
{
    return m_brockdepmin;
}
////////////////////////////////////////////////////////////////////////////////
Var const& MapUnit::GetLkey() const
{
    return m_lkey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& MapUnit::GetMuacres() const
{
    return m_muacres;
}
////////////////////////////////////////////////////////////////////////////////
Var const& MapUnit::GetMukey() const
{
    return m_mukey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& MapUnit::GetMuname() const
{
    return m_muname;
}
////////////////////////////////////////////////////////////////////////////////
Var const& MapUnit::GetMusym() const
{
    return m_musym;
}
////////////////////////////////////////////////////////////////////////////////
bool const& MapUnit::HasComponents() const
{
    return m_hasComponents;
}
////////////////////////////////////////////////////////////////////////////////
void MapUnit::Populate() const
{
    boost::call_once( m_of1,
        boost::bind( &MapUnit::PopulateComponents, this ) );
}
////////////////////////////////////////////////////////////////////////////////
void MapUnit::PopulateComponents() const
{
    ( GetSession( DB_SSURGO )
        << Component::INPUT << "\n"
        << "where\n"
        << "  component.mukey = ?",
        into( m_components ),
        useRef( m_mukey ),
        now );

    m_hasComponents = ( m_components.size() != 0 );

    ComponentsByCokey::const_iterator itr =
        m_components.get< COKEY >().begin();
    for( ; itr != m_components.get< COKEY >().end(); ++itr )
    {
        (*itr)->m_mapUnit = this;
        (*itr)->Populate();
    }
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    MapUnitPtr const& mapUnit )
{
    os
        << "MapUnit: " << &*mapUnit
        << std::endl
        << "  areaname_legend: "
        << Convert< std::string >( mapUnit->GetAreanameLegend(), "null" )
        << std::endl
        << "  areasymbol_legend: "
        << Convert< std::string >( mapUnit->GetAreasymbolLegend() )
        << std::endl
        << "  brockdepmin: "
        << Convert< std::string >( mapUnit->GetBrockdepmin(), "null" )
        << std::endl
        << "  lkey: "
        << Convert< std::string >( mapUnit->GetLkey() )
        << std::endl
        << "  muacres: "
        << Convert< std::string >( mapUnit->GetMuacres(), "null" )
        << std::endl
        << "  mukey: "
        << Convert< std::string >( mapUnit->GetMukey() )
        << std::endl
        << "  muname: "
        << Convert< std::string >( mapUnit->GetMuname(), "null" )
        << std::endl
        << "  musym: "
        << Convert< std::string >( mapUnit->GetMusym() )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
