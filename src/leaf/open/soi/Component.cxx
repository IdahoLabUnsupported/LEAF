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
#include <leaf/open/soi/Component.h>
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
std::string const Component::INPUT =
    "select\n"
    "  component.albedodry_r,\n"
    "  component.albedodry_l,\n"
    "  component.albedodry_h,\n"
    "  component.cokey,\n"
    "  component.compkind,\n"
    "  component.compname,\n"
    "  component.comppct_r,\n"
    "  component.comppct_l,\n"
    "  component.comppct_h,\n"
    "  component.geomdesc,\n"
    "  component.hydgrp,\n"
    "  component.localphase,\n"
    "  component.majcompflag,\n"
    "  component.mukey,\n"
    "  component.nirrcapcl,\n"
    "  component.slope_r,\n"
    "  component.slope_l,\n"
    "  component.slope_h,\n"
    "  component.slopelenusle_r,\n"
    "  component.slopelenusle_l,\n"
    "  component.slopelenusle_h,\n"
    "  component.taxorder,\n"
    "  component.taxsubgrp,\n"
    "  component.tfact\n"
    "from component";
////////////////////////////////////////////////////////////////////////////////
Component::Component()
    :
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_of2( BOOST_ONCE_INIT_CONST ),
    m_albedodry(),
    m_cokey(),
    m_compkind(),
    m_compname(),
    m_comppct(),
    m_geomdesc(),
    m_hydgrp(),
    m_localphase(),
    m_majcompflag(),
    m_mukey(),
    m_nirrcapcl(),
    m_slope(),
    m_slopelenusle(),
    m_taxorder(),
    m_taxsubgrp(),
    m_tfact(),
    m_mapUnit( NULL ),
    m_hasCorestrictions(),
    m_hasHorizons(),
    m_corestrictions(),
    m_horizons()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Component::~Component()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CorestrictionPtr Component::GetCorestriction(
    std::string const& corestrictkey,
    bool const& mustExist ) const
{
    CorestrictionsByCorestrictkey::const_iterator itr =
        m_corestrictions.get< CORESTRICTKEY >().find( corestrictkey );
    if( itr == m_corestrictions.get< CORESTRICTKEY >().end() )
    {
        if( !mustExist ) return CorestrictionPtr();

        std::stringstream err;
        err << "leaf::open::soi::Component::GetCorestriction"
            << "( \"" << corestrictkey << "\" ): "
            << "could not find specified corestrictkey";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > Component::GetMinResDept() const
{
    if( m_corestrictions.empty() )
    {
        return Nullable< Var >();
    }

    CorestrictionsByResdept::const_iterator itr =
        m_corestrictions.get< RESDEPT >().begin();
    assert( itr != m_corestrictions.get< RESDEPT >().end() );
    return (*itr)->GetResdept();
}
////////////////////////////////////////////////////////////////////////////////
Corestrictions const& Component::GetCorestrictions() const
{
    return m_corestrictions;
}
////////////////////////////////////////////////////////////////////////////////
std::size_t Component::GetNumCorestrictions() const
{
    return m_corestrictions.size();
}
////////////////////////////////////////////////////////////////////////////////
CHorizonPtr Component::GetHorizon(
    std::string const& chkey,
    bool const& mustExist ) const
{
    CHorizonsByChkey::const_iterator itr =
        m_horizons.get< CHKEY >().find( chkey );
    if( itr == m_horizons.get< CHKEY >().end() )
    {
        if( !mustExist ) return CHorizonPtr();

        std::stringstream err;
        err << "leaf::open::soi::Component::GetHorizon( \"" << chkey
            << "\" ) could not find specified chkey in CHorizon map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
CHorizons const& Component::GetHorizons() const
{
    return m_horizons;
}
////////////////////////////////////////////////////////////////////////////////
CHorizonsByHzdeptRange Component::GetHorizonsByHzdeptRange(
    unsigned int const& lowerBound,
    unsigned int const& upperBound ) const
{
    return m_horizons.get< HZDEPT >().range(
        boost::bind< bool >( util::greater_than_or_equal_to< int >(),
            boost::lambda::_1, lowerBound ),
        boost::bind< bool >( util::less_than_or_equal_to< int >(),
            boost::lambda::_1, upperBound ) );
}
////////////////////////////////////////////////////////////////////////////////
std::size_t Component::GetNumHorizons() const
{
    return m_horizons.size();
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetAlbedodry(
    unsigned int const& valType ) const
{
    return m_albedodry[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Var const& Component::GetCokey() const
{
    return m_cokey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetCompkind() const
{
    return m_compkind;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetCompname() const
{
    return m_compname;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetComppct_r() const
{
    return m_comppct[ SsurgoValType::R ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetComppct(
    unsigned int const& valType ) const
{
    return m_comppct[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetGeomdesc() const
{
    return m_geomdesc;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetHydgrp() const
{
    return m_hydgrp;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetLocalphase() const
{
    return m_localphase;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetMajcompflag() const
{
    return m_majcompflag;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Component::GetMukey() const
{
    return m_mukey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetNirrcapcl() const
{
    return m_nirrcapcl;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetSlope(
    unsigned int const& valType ) const
{
    return m_slope[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetSlopelenusle(
    unsigned int const& valType ) const
{
    return m_slopelenusle[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetTaxorder() const
{
    return m_taxorder;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetTaxsubgrp() const
{
    return m_taxsubgrp;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Component::GetTfact() const
{
    return m_tfact;
}
////////////////////////////////////////////////////////////////////////////////
MapUnit const& Component::GetMapUnit() const
{
    return *m_mapUnit;
}
////////////////////////////////////////////////////////////////////////////////
bool const& Component::HasCorestrictions() const
{
    return m_hasCorestrictions;
}
////////////////////////////////////////////////////////////////////////////////
bool const& Component::HasHorizons() const
{
    return m_hasHorizons;
}
////////////////////////////////////////////////////////////////////////////////
void Component::Populate() const
{
    boost::call_once( m_of1,
        boost::bind( &Component::PopulateCorestrictions, this ) );
    boost::call_once( m_of2,
        boost::bind( &Component::PopulateHorizons, this ) );
}
////////////////////////////////////////////////////////////////////////////////
void Component::PopulateCorestrictions() const
{
    ( GetSession( DB_SSURGO )
        << "select\n"
        << "  corestrictions.corestrictkey,\n"
        << "  corestrictions.resdept_r,\n"
        << "  corestrictions.resdept_l,\n"
        << "  corestrictions.resdept_h\n"
        << "from corestrictions\n"
        << "where\n"
        << "  corestrictions.cokey = ?",
        into( m_corestrictions ),
        useRef( m_cokey ),
        now );

    m_hasCorestrictions = ( m_corestrictions.size() != 0 );
}
////////////////////////////////////////////////////////////////////////////////
void Component::PopulateHorizons() const
{
    ( GetSession( DB_SSURGO )
        << CHorizon::INPUT << "\n"
        << "where\n"
        << "  chorizon.cokey = ? and\n"
        << "( chorizon.hzdept_r is not null or\n"
        << "( chorizon.hzdept_l is not null and\n"
        << "  chorizon.hzdept_h is not null ) ) and\n"
        << "( chorizon.hzdepb_r is not null or\n"
        << "( chorizon.hzdepb_l is not null and\n"
        << "  chorizon.hzdepb_h is not null ) )",
        into( m_horizons ),
        useRef( m_cokey ),
        now );

    m_hasHorizons = ( m_horizons.size() != 0 );

    //Decide on the appropriate aggregation to be used to summarize horizons
    //depth weighted:
    //  to calculate average percent clay through horizons
    //top 1m:
    //  to summarize shrink-swell capacity of the topsoil
    //top horizon:
    //  to summarize surface organic carbon
    //profile sum:
    //  to calculate total water holding capacity
    //most limiting:
    //  to calculate most limiting hydraulic conductivity within soil profile
    //Aggregate horizon data: after filtering NULL values and weights
    CHorizonsByChkey::const_iterator itr =
        m_horizons.get< CHKEY >().begin();
    for( ; itr != m_horizons.get< CHKEY >().end(); ++itr )
    {
        (*itr)->Populate();
    }
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ComponentPtr const& component )
{
    os
        << "Component: " << &*component
        << std::endl
        << "  albedodry: "
        << Convert< std::string >( component->GetAlbedodry(), "null" )
        << std::endl
        << "  cokey: "
        << Convert< std::string >( component->GetCokey() )
        << std::endl
        << "  compkind: "
        << Convert< std::string >( component->GetCompkind(), "null" )
        << std::endl
        << "  compname: "
        << Convert< std::string >( component->GetCompname(), "null" )
        << std::endl
        << "  comppct: "
        << Convert< std::string >( component->GetComppct(), "null" )
        << std::endl
        << "  geomdesc: "
        << Convert< std::string >( component->GetGeomdesc(), "null" )
        << std::endl
        << "  hydgrp: "
        << Convert< std::string >( component->GetHydgrp(), "null" )
        << std::endl
        << "  localphase: "
        << Convert< std::string >( component->GetLocalphase(), "null" )
        << std::endl
        << "  majcompflag: "
        << Convert< std::string >( component->GetMajcompflag(), "null" )
        << std::endl
        << "  mukey: "
        << Convert< std::string >( component->GetMukey() )
        << std::endl
        << "  nirrcapcl: "
        << Convert< std::string >( component->GetNirrcapcl(), "null" )
        << std::endl
        << "  slope: "
        << Convert< std::string >( component->GetSlope(), "null" )
        << std::endl
        << "  slopelenusle: "
        << Convert< std::string >( component->GetSlopelenusle(), "null" )
        << std::endl
        << "  taxorder: "
        << Convert< std::string >( component->GetTaxorder(), "null" )
        << std::endl
        << "  taxsubgrp: "
        << Convert< std::string >( component->GetTaxsubgrp(), "null" )
        << std::endl
        << "  tfact: "
        << Convert< std::string >( component->GetTfact(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
