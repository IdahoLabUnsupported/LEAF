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
#include <leaf/open/scn/Scenario.h>

using namespace leaf::util;

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/regex.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

// --- Poco Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace scn
{

////////////////////////////////////////////////////////////////////////////////
Scenario::Scenario()
    :
    Object(),
    m_id(),
    m_name(),
    m_spatial(),
    m_climate(),
    m_soil(),
    m_rotation(),
    m_cover(),
    m_tillage(),
    m_removal(),
    m_barrier(),
    m_yield()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Scenario::Scenario(
    Var const& name,
    Var const& soil,
    Var const& rotation,
    Var const& tillage,
    Var const& removal,
    Var const& yield,
    Nullable< Var > const& spatial,
    Nullable< Var > const& climate,
    Nullable< Var > const& cover,
    Nullable< Var > const& barrier,
    Nullable< Var > const& id )
    :
    Object(),
    m_id(),
    m_name( name ),
    m_spatial( spatial ),
    m_climate( climate ),
    m_soil( soil ),
    m_rotation( rotation ),
    m_cover( cover ),
    m_tillage( tillage ),
    m_removal( removal ),
    m_barrier( barrier ),
    m_yield( yield )
{
    CheckIfExists( id );
}
////////////////////////////////////////////////////////////////////////////////
Scenario::~Scenario()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ScenarioPtr Scenario::Create()
{
    return boost::make_shared< Scenario >();
}
////////////////////////////////////////////////////////////////////////////////
ScenarioPtr Scenario::Create(
    Var const& name,
    Var const& soil,
    Var const& rotation,
    Var const& tillage,
    Var const& removal,
    Var const& yield,
    Nullable< Var > const& spatial,
    Nullable< Var > const& climate,
    Nullable< Var > const& cover,
    Nullable< Var > const& barrier,
    Nullable< Var > const& id )
{
    //Can't use boost::make_shared here as it's limited to 9 arguments
    return ScenarioPtr( new Scenario(
        name, soil, rotation, tillage, removal,
        yield, spatial, climate, cover, barrier, id ) );
}
////////////////////////////////////////////////////////////////////////////////
void Scenario::CheckIfExists(
    Nullable< Var > const& id )
{
    Var newId; if( !id.isNull() ) newId = id.value();
    bool validNewId( false );
    if( !newId.isEmpty() )
    {
        //Verify that we have a valid RFC4122 uuid
        static boost::regex const uuidRegex(
            "^[0-9a-f]{8}-[0-9a-f]{4}-"
            "[1-5][0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$",
            boost::regex::perl | boost::regex_constants::icase );
        validNewId = boost::regex_match(
            newId.extract< std::string >(), uuidRegex );
        if( !validNewId )
        {
            throw std::runtime_error(
                "leaf::open::scn::Scenario::CheckIfExists()\n"
                "  desired id is not a valid RFC4122 uuid" );
        }

        //Make sure that the new id does not already exist in the database
        validNewId = !Exists( newId );
        if( !validNewId )
        {
            throw std::runtime_error(
                "leaf::open::scn::Scenario::CheckIfExists()\n"
                "  desired id already exists for a different scenario" );
        }
    }

    Nullable< Var > curId;
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  t1.id\n"
        << "from " << ScnTbl() << " as t1\n"
        << "where\n"
        << "  t1.name = ? and\n"
        << "((t1.spatial is null and ? is null) or\n"
        << " (t1.spatial = ?)) and\n"
        << "((t1.climate is null and ? is null) or\n"
        << " (t1.climate = ?)) and\n"
        << "  t1.soil = ? and\n"
        << "  t1.rotation = ? and\n"
        << "((t1.cover is null and ? is null) or\n"
        << " (t1.cover = ?)) and\n"
        << "  t1.tillage = ? and\n"
        << "  t1.removal = ? and\n"
        << "((t1.barrier is null and ? is null) or\n"
        << " (t1.barrier = ?)) and\n"
        << "  t1.yield = ?",
        into( curId ),
        //There is no "Binding Parameters by Name" in ODBC...
        useRef( m_name ),
        useRef( m_spatial ),
        useRef( m_spatial ),
        useRef( m_climate ),
        useRef( m_climate ),
        useRef( m_soil ),
        useRef( m_rotation ),
        useRef( m_cover ),
        useRef( m_cover ),
        useRef( m_tillage ),
        useRef( m_removal ),
        useRef( m_barrier ),
        useRef( m_barrier ),
        useRef( m_yield );
    ExecuteRetry( stmtObj );
    bool curIdExists = ( !curId.isNull() );

    if( curIdExists )
    {
        if( validNewId )
        {
            throw std::runtime_error(
                "leaf::open::scn::Scenario::CheckIfExists()\n"
                "  scenario already exists for a different id" );
        }

        m_id = curId.value();
        return;
    }

    if( validNewId )
    {
        m_id = newId;
    }
    else
    {
        //When using multiple threads; will need to lock uuid generation
        boost::uuids::uuid tag = boost::uuids::random_generator()();
        m_id = boost::uuids::to_string( tag );
    }

    WriteToDB();
}
////////////////////////////////////////////////////////////////////////////////
void Scenario::DeleteFromDB(
    Var const& id )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "delete from " << ScnTbl() << "\n"
        << "where\n"
        << "  id = ?",
        useRef( id );
    ExecuteRetry( stmtObj );
}
////////////////////////////////////////////////////////////////////////////////
bool Scenario::Exists(
    Var const& id )
{
    unsigned int count( 0 );

    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  count( * )\n"
        << "from " << ScnTbl() << " as t1\n"
        << "where\n"
        << "  t1.id = ?",
        into( count ),
        useRef( id );
    ExecuteRetry( stmtObj );

    return ( count > 0 ) ? true : false;
}
////////////////////////////////////////////////////////////////////////////////
ScenarioPtr Scenario::Extract(
    Var const& id )
{
    unsigned int count( 0 );
    ScenarioPtr scenario = Create();

    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "select\n"
        << "  1 as count,\n"
        << "  t1.id,\n"
        << "  t1.name,\n"
        << "  t1.spatial,\n"
        << "  t1.climate,\n"
        << "  t1.soil,\n"
        << "  t1.rotation,\n"
        << "  t1.cover,\n"
        << "  t1.tillage,\n"
        << "  t1.removal,\n"
        << "  t1.barrier,\n"
        << "  t1.yield\n"
        << "from " << ScnTbl() << " as t1\n"
        << "where\n"
        << "  t1.id = ?",
        into( count, 0 ),
        into( *scenario ),
        useRef( id );
    ExecuteRetry( stmtObj );

    return ( count > 0 ) ? scenario : ScenarioPtr();
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetId() const
{
    return m_id;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetName() const
{
    return m_name;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Scenario::GetSpatial() const
{
    return m_spatial;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Scenario::GetClimate() const
{
    return m_climate;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetSoil() const
{
    return m_soil;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetRotation() const
{
    return m_rotation;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Scenario::GetCover() const
{
    return m_cover;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetTillage() const
{
    return m_tillage;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetRemoval() const
{
    return m_removal;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Scenario::GetBarrier() const
{
    return m_barrier;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Scenario::GetYield() const
{
    return m_yield;
}
////////////////////////////////////////////////////////////////////////////////
void Scenario::WriteToDB()
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "insert into " << ScnTbl() << "\n"
        << "values(\n"
        << "  ? /*id*/,\n"
        << "  ? /*name*/,\n"
        << "  ? /*spatial*/,\n"
        << "  ? /*climate*/,\n"
        << "  ? /*soil*/,\n"
        << "  ? /*rotation*/,\n"
        << "  ? /*cover*/,\n"
        << "  ? /*tillage*/,\n"
        << "  ? /*removal*/,\n"
        << "  ? /*barrier*/,\n"
        << "  ? /*yield*/ )",
        useRef( *this );
    ExecuteRetry( stmtObj );
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ScenarioPtr const& scenario )
{
    os
        << "Scenario: " << &*scenario
        << std::endl
        << "  id: "
        << Convert< std::string >( scenario->GetId() )
        << std::endl
        << "  name: "
        << Convert< std::string >( scenario->GetName() )
        << std::endl
        << "  spatial: "
        << Convert< std::string >( scenario->GetSpatial(), "null" )
        << std::endl
        << "  climate: "
        << Convert< std::string >( scenario->GetClimate(), "null" )
        << std::endl
        << "  soil: "
        << Convert< std::string >( scenario->GetSoil() )
        << std::endl
        << "  rotation: "
        << Convert< std::string >( scenario->GetRotation() )
        << std::endl
        << "  cover: "
        << Convert< std::string >( scenario->GetCover(), "null" )
        << std::endl
        << "  tillage: "
        << Convert< std::string >( scenario->GetTillage() )
        << std::endl
        << "  removal: "
        << Convert< std::string >( scenario->GetRemoval() )
        << std::endl
        << "  barrier: "
        << Convert< std::string >( scenario->GetBarrier(), "null" )
        << std::endl
        << "  yield: "
        << Convert< std::string >( scenario->GetYield() )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end scn
} //end open
} //end leaf
