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
#include <leaf/open/scn/Common.h>

using namespace leaf::util;

// --- POCO Includes --- //
using namespace Poco::Data;
using namespace Keywords;

namespace leaf
{
namespace open
{
namespace scn
{

////////////////////////////////////////////////////////////////////////////////
std::string ScnTbl()
{
    static std::string tableName = "scenario";
    static bool ptr( CreateScenarioTable( tableName ) );

    return tableName;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateScenarioTable(
    std::string const& tableName )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "create table if not exists " << tableName.c_str() << "(\n"
        << "  id char( 36 ) not null,\n"
        << "  name varchar( 50 ) not null,\n"
        << "  spatial integer,\n"
        << "  climate varchar( 50 ),\n"
        << "  soil varchar( 50 ) not null,\n"
        << "  rotation varchar( 50 ) not null,\n"
        << "  cover varchar( 50 ),\n"
        << "  tillage varchar( 50 ) not null,\n"
        << "  removal varchar( 50 ) not null,\n"
        << "  barrier varchar( 50 ),\n"
        << "  yield varchar( 50 ) not null,\n"
        << "constraint " << tableName.c_str() << "_pkey\n"
        << "  primary key(\n"
        << "    id ),\n"
        << "constraint " << tableName.c_str() << "_ukey\n"
        << "  unique(\n"
        << "    name,\n"
        << "    spatial,\n"
        << "    climate,\n"
        << "    soil,\n"
        << "    rotation,\n"
        << "    cover,\n"
        << "    tillage,\n"
        << "    removal,\n"
        << "    barrier,\n"
        << "    yield ) );";
    ExecuteRetry( stmtObj );

    return true;
}
////////////////////////////////////////////////////////////////////////////////

} //end scn
} //end open
} //end leaf
