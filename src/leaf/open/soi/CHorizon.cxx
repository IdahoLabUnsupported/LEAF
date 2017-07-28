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
#include <leaf/open/soi/CHorizon.h>

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
std::string const CHorizon::INPUT =
    "select\n"
    "  chorizon.caco3_r,\n"
    "  chorizon.caco3_l,\n"
    "  chorizon.caco3_h,\n"
    "  chorizon.cec7_r,\n"
    "  chorizon.cec7_l,\n"
    "  chorizon.cec7_h,\n"
    "  chorizon.chkey,\n"
    "  chorizon.claytotal_r,\n"
    "  chorizon.claytotal_l,\n"
    "  chorizon.claytotal_h,\n"
    "  chorizon.cokey,\n"
    "  chorizon.dbthirdbar_r,\n"
    "  chorizon.dbthirdbar_l,\n"
    "  chorizon.dbthirdbar_h,\n"
    "  chorizon.desgnmaster,\n"
    "  chorizon.ecec_r,\n"
    "  chorizon.ecec_l,\n"
    "  chorizon.ecec_h,\n"
    "  chorizon.hzdepb_r,\n"
    "  chorizon.hzdepb_l,\n"
    "  chorizon.hzdepb_h,\n"
    "  chorizon.hzdept_r,\n"
    "  chorizon.hzdept_l,\n"
    "  chorizon.hzdept_h,\n"
    "  chorizon.hzname,\n"
    "  chorizon.hzthk_r,\n"
    "  chorizon.hzthk_l,\n"
    "  chorizon.hzthk_h,\n"
    "  chorizon.kffact,\n"
    "  chorizon.ksat_r,\n"
    "  chorizon.ksat_l,\n"
    "  chorizon.ksat_h,\n"
    "  chorizon.lep_r,\n"
    "  chorizon.lep_l,\n"
    "  chorizon.lep_h,\n"
    "  chorizon.om_r,\n"
    "  chorizon.om_l,\n"
    "  chorizon.om_h,\n"
    "  chorizon.ph01mcacl2_r,\n"
    "  chorizon.ph01mcacl2_l,\n"
    "  chorizon.ph01mcacl2_h,\n"
    "  chorizon.ph1to1h2o_r,\n"
    "  chorizon.ph1to1h2o_l,\n"
    "  chorizon.ph1to1h2o_h,\n"
    "  chorizon.sandco_r,\n"
    "  chorizon.sandco_l,\n"
    "  chorizon.sandco_h,\n"
    "  chorizon.sandfine_r,\n"
    "  chorizon.sandfine_l,\n"
    "  chorizon.sandfine_h,\n"
    "  chorizon.sandmed_r,\n"
    "  chorizon.sandmed_l,\n"
    "  chorizon.sandmed_h,\n"
    "  chorizon.sandtotal_r,\n"
    "  chorizon.sandtotal_l,\n"
    "  chorizon.sandtotal_h,\n"
    "  chorizon.sandvc_r,\n"
    "  chorizon.sandvc_l,\n"
    "  chorizon.sandvc_h,\n"
    "  chorizon.sandvf_r,\n"
    "  chorizon.sandvf_l,\n"
    "  chorizon.sandvf_h,\n"
    "  chorizon.silttotal_r,\n"
    "  chorizon.silttotal_l,\n"
    "  chorizon.silttotal_h,\n"
    "  chorizon.wfifteenbar_r,\n"
    "  chorizon.wfifteenbar_l,\n"
    "  chorizon.wfifteenbar_h,\n"
    "  chorizon.wtenthbar_r,\n"
    "  chorizon.wtenthbar_l,\n"
    "  chorizon.wtenthbar_h,\n"
    "  chorizon.wthirdbar_r,\n"
    "  chorizon.wthirdbar_l,\n"
    "  chorizon.wthirdbar_h\n"
    "from chorizon";
////////////////////////////////////////////////////////////////////////////////
CHorizon::CHorizon()
    :
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_of2( BOOST_ONCE_INIT_CONST ),
    m_caco3(),
    m_cec7(),
    m_chkey(),
    m_claytotal(),
    m_cokey(),
    m_dbthirdbar(),
    m_desgnmaster(),
    m_ecec(),
    m_hzdepb(),
    m_hzdept(),
    m_hzname(),
    m_hzthk(),
    m_kffact(),
    m_ksat(),
    m_lep(),
    m_om(),
    m_ph01mcacl2(),
    m_ph1to1h2o(),
    m_sandco(),
    m_sandfine(),
    m_sandmed(),
    m_sandtotal(),
    m_sandvc(),
    m_sandvf(),
    m_silttotal(),
    m_wfifteenbar(),
    m_wtenthbar(),
    m_wthirdbar(),
    m_hasFrags( false ),
    m_hasTextureGrps( false ),
    m_frags(),
    m_textureGrps()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CHorizon::~CHorizon()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ChFragPtr CHorizon::GetFrag(
    std::string const& chfragskey,
    bool const& mustExist ) const
{
    ChFragsByChfragskey::const_iterator itr =
        m_frags.get< CHFRAGSKEY >().find( chfragskey );
    if( itr == m_frags.get< CHFRAGSKEY >().end() )
    {
        if( !mustExist ) return ChFragPtr();

        std::stringstream err;
        err << "leaf::open::soi::CHorizon::GetFrag( \"" << chfragskey
            << "\" ) could not find specified chfragskey in ChFrag map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
ChFrags const& CHorizon::GetFrags() const
{
    return m_frags;
}
////////////////////////////////////////////////////////////////////////////////
std::size_t CHorizon::GetNumFrags() const
{
    return m_frags.size();
}
////////////////////////////////////////////////////////////////////////////////
ChTextureGrpPtr CHorizon::GetTextureGrp(
    std::string const& chtgkey,
    bool const& mustExist ) const
{
    ChTextureGrpsByChtgkey::const_iterator itr =
        m_textureGrps.get< CHTGKEY >().find( chtgkey );
    if( itr == m_textureGrps.get< CHTGKEY >().end() )
    {
        if( !mustExist ) return ChTextureGrpPtr();

        std::stringstream err;
        err << "leaf::open::soi::CHorizon::GetTextureGrp( \"" << chtgkey
            << "\" ) could not find specified chtgkey in ChTextureGrp map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
ChTextureGrps const& CHorizon::GetTextureGrps() const
{
    return m_textureGrps;
}
////////////////////////////////////////////////////////////////////////////////
std::size_t CHorizon::GetNumTextureGrps() const
{
    return m_textureGrps.size();
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetCaco3(
    unsigned int const& valType ) const
{
    return m_caco3[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetCec7(
    unsigned int const& valType ) const
{
    return m_cec7[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Var const& CHorizon::GetChkey() const
{
    return m_chkey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetClaytotal(
    unsigned int const& valType ) const
{
    return m_claytotal[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Var const& CHorizon::GetCokey() const
{
    return m_cokey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetDbthirdbar(
    unsigned int const& valType ) const
{
    return m_dbthirdbar[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetDesgnmaster() const
{
    return m_desgnmaster;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetEcec(
    unsigned int const& valType ) const
{
    return m_ecec[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetHzdepb(
    unsigned int const& valType ) const
{
    return m_hzdepb[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetHzdept_r() const
{
    return m_hzdept[ SsurgoValType::R ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetHzdept(
    unsigned int const& valType ) const
{
    return m_hzdept[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetHzname() const
{
    return m_hzname;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetHzthk(
    unsigned int const& valType ) const
{
    return m_hzthk[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetKffact() const
{
    return m_kffact;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetKsat(
    unsigned int const& valType ) const
{
    return m_ksat[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetLep(
    unsigned int const& valType ) const
{
    return m_lep[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetOm(
    unsigned int const& valType ) const
{
    return m_om[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetPh01mcacl2(
    unsigned int const& valType ) const
{
    return m_ph01mcacl2[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetPh1to1h2o(
    unsigned int const& valType ) const
{
    return m_ph1to1h2o[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandco(
    unsigned int const& valType ) const
{
    return m_sandco[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandfine(
    unsigned int const& valType ) const
{
    return m_sandfine[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandmed(
    unsigned int const& valType ) const
{
    return m_sandmed[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandtotal(
    unsigned int const& valType ) const
{
    return m_sandtotal[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandvc(
    unsigned int const& valType ) const
{
    return m_sandvc[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSandvf(
    unsigned int const& valType ) const
{
    return m_sandvf[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetSilttotal(
    unsigned int const& valType ) const
{
    return m_silttotal[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetWfifteenbar(
    unsigned int const& valType ) const
{
    return m_wfifteenbar[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetWtenthbar(
    unsigned int const& valType ) const
{
    return m_wtenthbar[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& CHorizon::GetWthirdbar(
    unsigned int const& valType ) const
{
    return m_wthirdbar[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
bool const& CHorizon::HasFrags() const
{
    return m_hasFrags;
}
////////////////////////////////////////////////////////////////////////////////
bool const& CHorizon::HasTextureGrps() const
{
    return m_hasTextureGrps;
}
////////////////////////////////////////////////////////////////////////////////
void CHorizon::Populate() const
{
    boost::call_once( m_of1,
        boost::bind( &CHorizon::PopulateFrags, this ) );
    boost::call_once( m_of2,
        boost::bind( &CHorizon::PopulateTextureGrps, this ) );
}
////////////////////////////////////////////////////////////////////////////////
void CHorizon::PopulateFrags() const
{
    ( GetSession( DB_SSURGO )
        << "select\n"
        << "  t1.chfragskey,\n"
        << "  t1.fragvol_r,\n"
        << "  t1.fragvol_l,\n"
        << "  t1.fragvol_h\n"
        << "from chfrags as t1\n"
        << "where\n"
        << "  t1.chkey = ?",
        into( m_frags ),
        useRef( m_chkey ),
        now );

    m_hasFrags = ( m_frags.size() != 0 );
}
////////////////////////////////////////////////////////////////////////////////
void CHorizon::PopulateTextureGrps() const
{
    ( GetSession( DB_SSURGO )
        << "select\n"
        << "  t1.chtgkey,\n"
        << "  t1.rvindicator,\n"
        << "  t1.stratextsflag,\n"
        << "  t1.texdesc,\n"
        << "  t1.texture\n"
        << "from chtexturegrp as t1\n"
        << "where\n"
        << "  t1.chkey = ?",
        into( m_textureGrps ),
        useRef( m_chkey ),
        now );

     m_hasTextureGrps = ( m_textureGrps.size() != 0 );

    ChTextureGrpsByChtgkey::const_iterator itr =
        m_textureGrps.get< CHTGKEY >().begin();
    for( ; itr != m_textureGrps.get< CHTGKEY >().end(); ++itr )
    {
        (*itr)->Populate();
    }
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    CHorizonPtr const& chorizon )
{
    os
        << "CHorizon: " << &*chorizon
        << std::endl
        << "  caco3: "
        << Convert< std::string >( chorizon->GetCaco3(), "null" )
        << std::endl
        << "  cec7: "
        << Convert< std::string >( chorizon->GetCec7(), "null" )
        << std::endl
        << "  chkey: "
        << Convert< std::string >( chorizon->GetChkey() )
        << std::endl
        << "  claytotal: "
        << Convert< std::string >( chorizon->GetClaytotal(), "null" )
        << std::endl
        << "  cokey: "
        << Convert< std::string >( chorizon->GetCokey() )
        << std::endl
        << "  dbthirdbar: "
        << Convert< std::string >( chorizon->GetDbthirdbar(), "null" )
        << std::endl
        << "  desgnmaster: "
        << Convert< std::string >( chorizon->GetDesgnmaster(), "null" )
        << std::endl
        << "  ecec: "
        << Convert< std::string >( chorizon->GetEcec(), "null" )
        << std::endl
        << "  hzdepb: "
        << Convert< std::string >( chorizon->GetHzdepb(), "null" )
        << std::endl
        << "  hzdept: "
        << Convert< std::string >( chorizon->GetHzdept(), "null" )
        << std::endl
        << "  hzname: "
        << Convert< std::string >( chorizon->GetHzname(), "null" )
        << std::endl
        << "  hzthk: "
        << Convert< std::string >( chorizon->GetHzthk(), "null" )
        << std::endl
        << "  kffact: "
        << Convert< std::string >( chorizon->GetKffact(), "null" )
        << std::endl
        << "  ksat: "
        << Convert< std::string >( chorizon->GetKsat(), "null" )
        << std::endl
        << "  lep: "
        << Convert< std::string >( chorizon->GetLep(), "null" )
        << std::endl
        << "  om: "
        << Convert< std::string >( chorizon->GetOm(), "null" )
        << std::endl
        << "  ph01mcacl2: "
        << Convert< std::string >( chorizon->GetPh01mcacl2(), "null" )
        << std::endl
        << "  ph1to1h2o: "
        << Convert< std::string >( chorizon->GetPh1to1h2o(), "null" )
        << std::endl
        << "  sandco: "
        << Convert< std::string >( chorizon->GetSandco(), "null" )
        << std::endl
        << "  sandfine: "
        << Convert< std::string >( chorizon->GetSandfine(), "null" )
        << std::endl
        << "  sandmed: "
        << Convert< std::string >( chorizon->GetSandmed(), "null" )
        << std::endl
        << "  sandtotal: "
        << Convert< std::string >( chorizon->GetSandtotal(), "null" )
        << std::endl
        << "  sandvc: "
        << Convert< std::string >( chorizon->GetSandvc(), "null" )
        << std::endl
        << "  sandvf: "
        << Convert< std::string >( chorizon->GetSandvf(), "null" )
        << std::endl
        << "  silttotal: "
        << Convert< std::string >( chorizon->GetSilttotal(), "null" )
        << std::endl
        << "  wfifteenbar: "
        << Convert< std::string >( chorizon->GetWfifteenbar(), "null" )
        << std::endl
        << "  wtenthbar: "
        << Convert< std::string >( chorizon->GetWtenthbar(), "null" )
        << std::endl
        << "  wthirdbar: "
        << Convert< std::string >( chorizon->GetWthirdbar(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
