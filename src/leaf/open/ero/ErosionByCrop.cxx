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
#include <leaf/open/ero/ErosionByCrop.h>
#include <leaf/open/ero/Erosion.h>

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
ErosionByCrop::ErosionByCrop()
    :
    Object(),
    m_type(),
    m_scenarioId(),
    m_crop(),
    m_totBioRem(),
    m_rnk()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ErosionByCrop::ErosionByCrop(
    ErosionPtr const& erosion,
    std::string const& crop,
    double const& totBioRem,
    unsigned int const& rnk )
    :
    Object(),
    m_type( erosion->GetType() ),
    m_scenarioId( erosion->GetScenarioId() ),
    m_crop( crop ),
    m_totBioRem( totBioRem ),
    m_rnk( rnk )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ErosionByCropPtr ErosionByCrop::Create(
    ErosionPtr const& erosion,
    std::string const& crop,
    double const& totBioRem,
    unsigned int const& rnk )
{
    ErosionByCropPtr eroByCrop =
        boost::make_shared< ErosionByCrop >( erosion, crop, totBioRem, rnk );
    erosion->PushBack( eroByCrop );

    return eroByCrop;
}
////////////////////////////////////////////////////////////////////////////////
ErosionByCrop::~ErosionByCrop()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ErosionByCrops ErosionByCrop::Extract(
    ErosionPtr const& erosion )
{
    ErosionByCrops eroByCrops;

    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  t1.scenario_id,\n"
        << "  t1.crop,\n"
        << "  t1.totbiorem,\n"
        << "  t1.rnk\n"
        << "from " << EroByCrpTbl( erosion->GetType() ) << " as t1\n"
        << "where\n"
        << "  t1.scenario_id = ?",
        into( eroByCrops ),
        useRef( erosion->GetScenarioId() );
    ExecuteRetry( stmtObj );

    ErosionByCrops::iterator itr = eroByCrops.begin();
    for( ; itr != eroByCrops.end(); ++itr )
    {
        ErosionByCropPtr& eroByCrop = *itr;
        eroByCrop->m_type = erosion->GetType();
    }

    return eroByCrops;
}
////////////////////////////////////////////////////////////////////////////////
Type const& ErosionByCrop::GetType() const
{
    return m_type;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ErosionByCrop::GetScenarioId() const
{
    return m_scenarioId;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ErosionByCrop::GetCrop() const
{
    return m_crop;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ErosionByCrop::GetTotBioRem() const
{
    return m_totBioRem;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ErosionByCrop::GetRnk() const
{
    return m_rnk;
}
////////////////////////////////////////////////////////////////////////////////
void ErosionByCrop::WriteToDB()
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "insert into " << EroByCrpTbl( m_type ) << "(\n"
        << "  scenario_id,\n"
        << "  crop,\n"
        << "  totbiorem,\n"
        << "  rnk )\n"
        << "values(\n"
        << "  ? /*scenario_id*/,\n"
        << "  ? /*crop*/,\n"
        << "  ? /*totbiorem*/,\n"
        << "  ? /*rnk*/ )",
        useRef( *this );
    ExecuteRetry( stmtObj );
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ErosionByCropPtr const& eroByCrop )
{
    os
        << "ErosionByCrop: " << &*eroByCrop
        << std::endl
        << "  scenario_id: "
        << Convert< std::string >( eroByCrop->GetScenarioId() )
        << std::endl
        << "  crop: "
        << Convert< std::string >( eroByCrop->GetCrop() )
        << std::endl
        << "  totBioRem: "
        << Convert< std::string >( eroByCrop->GetTotBioRem() )
        << std::endl
        << "  rnk: "
        << Convert< std::string >( eroByCrop->GetRnk() )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end erosion
} //end open
} //end leaf
