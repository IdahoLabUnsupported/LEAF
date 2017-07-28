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
#include <leaf/open/ero/Erosion.h>

#include <leaf/open/scn/Scenario.h>

using namespace leaf::util;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace ero
{

////////////////////////////////////////////////////////////////////////////////
Erosion::Erosion()
    :
    Object(),
    m_type(),
    m_scenarioId(),
    m_sci(),
    m_sciOM(),
    m_sciER(),
    m_sciFO(),
    m_windEros(),
    m_waterEros(),
    m_totBioRem(),
    m_eroByCrops()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Erosion::Erosion(
    Type const& type,
    scn::ScenarioPtr const& scenario )
    :
    Object(),
    m_type( type ),
    m_scenarioId( scenario->GetId() ),
    m_sci(),
    m_sciOM(),
    m_sciER(),
    m_sciFO(),
    m_windEros(),
    m_waterEros(),
    m_totBioRem(),
    m_eroByCrops()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Erosion::~Erosion()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ErosionPtr Erosion::Create(
    Type const& type,
    scn::ScenarioPtr const& scenario )
{
    return boost::make_shared< Erosion >( type, scenario );
}
////////////////////////////////////////////////////////////////////////////////
bool Erosion::Exists(
    Type const& type,
    scn::ScenarioPtr const& scenario )
{
    unsigned int count( 0 );

    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  count( * )\n"
        << "from " << EroTbl( type ) << " as t1\n"
        << "where\n"
        << "  t1.scenario_id = ?",
        into( count ),
        useRef( scenario->GetId() );
    ExecuteRetry( stmtObj );

    return ( count > 0 ) ? true : false;
}
////////////////////////////////////////////////////////////////////////////////
ErosionPtr Erosion::Extract(
    Type const& type,
    scn::ScenarioPtr const& scenario )
{
    unsigned int count( 0 );
    ErosionPtr erosion = Create( type, scenario );

    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  1 as count,\n"
        << "  t1.scenario_id,\n"
        << "  t1.sci,\n"
        << "  t1.sciom,\n"
        << "  t1.scier,\n"
        << "  t1.scifo,\n"
        << "  t1.winderos,\n"
        << "  t1.watereros,\n"
        << "  t1.totbiorem\n"
        << "from " << EroTbl( type ) << " as t1\n"
        << "where\n"
        << "  t1.scenario_id = ?",
        into( count, 0 ),
        into( *erosion ),
        useRef( scenario->GetId() );
    ExecuteRetry( stmtObj );

    erosion->SetEroByCropVec( ErosionByCrop::Extract( erosion ) );

    return ( count > 0 ) ? erosion : ErosionPtr();
}
////////////////////////////////////////////////////////////////////////////////
Type const& Erosion::GetType() const
{
    return m_type;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetScenarioId() const
{
    return m_scenarioId;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetSci() const
{
    return m_sci;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetSciOM() const
{
    return m_sciOM;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetSciER() const
{
    return m_sciER;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetSciFO() const
{
    return m_sciFO;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetWindEros() const
{
    return m_windEros;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetWaterEros() const
{
    return m_waterEros;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Erosion::GetTotBioRem() const
{
    return m_totBioRem;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::PushBack(
    ErosionByCropPtr const& eroByCrop )
{
    m_eroByCrops.push_back( eroByCrop );
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetSci(
    Var const& sci )
{
    m_sci = sci;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetSciOM(
    Var const& sciOM )
{
    m_sciOM = sciOM;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetSciER(
    Var const& sciER )
{
    m_sciER = sciER;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetSciFO(
    Var const& sciFO )
{
    m_sciFO = sciFO;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetWindEros(
    Var const& windEros )
{
    m_windEros = windEros;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetWaterEros(
    Var const& waterEros )
{
    m_waterEros = waterEros;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetTotBioRem(
    Var const& totBioRem )
{
    m_totBioRem = totBioRem;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::SetEroByCropVec(
    ErosionByCrops const& eroByCrops )
{
    m_eroByCrops = eroByCrops;
}
////////////////////////////////////////////////////////////////////////////////
void Erosion::WriteToDB()
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "insert into " << EroTbl( m_type ) << "\n"
        << "values(\n"
        << "  ? /*scenario_id*/,\n"
        << "  ? /*sci*/,\n"
        << "  ? /*sciom*/,\n"
        << "  ? /*scier*/,\n"
        << "  ? /*scifo*/,\n"
        << "  ? /*winderos*/,\n"
        << "  ? /*watereros*/,\n"
        << "  ? /*totbiorem*/ )",
        useRef( *this );
    ExecuteRetry( stmtObj );

    ErosionByCrops::iterator itr = m_eroByCrops.begin();
    for( ; itr != m_eroByCrops.end(); ++itr )
    {
        ErosionByCropPtr const& eroByCrop = *itr;
        eroByCrop->WriteToDB();
    }
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ErosionPtr const& erosion )
{
    os
        << "Erosion: " << &*erosion
        << std::endl
        << "  scenario_id: "
        << Convert< std::string >( erosion->GetScenarioId() )
        << std::endl
        << "  sci: "
        << Convert< std::string >( erosion->GetSci() )
        << std::endl
        << "  sciOM: "
        << Convert< std::string >( erosion->GetSciOM() )
        << std::endl
        << "  sciER: "
        << Convert< std::string >( erosion->GetSciER() )
        << std::endl
        << "  sciFO: "
        << Convert< std::string >( erosion->GetSciFO() )
        << std::endl
        << "  windEros: "
        << Convert< std::string >( erosion->GetWindEros() )
        << std::endl
        << "  waterEros: "
        << Convert< std::string >( erosion->GetWaterEros() )
        << std::endl
        << "  totBioRem: "
        << Convert< std::string >( erosion->GetTotBioRem() )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end ero
} //end open
} //end leaf
