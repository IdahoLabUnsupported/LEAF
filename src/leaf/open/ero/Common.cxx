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
#include <leaf/open/ero/Common.h>

using namespace leaf::util;

// --- POCO Includes --- //
using namespace Poco::Data;
using namespace Keywords;

namespace leaf
{
namespace open
{
namespace ero
{

////////////////////////////////////////////////////////////////////////////////
std::string EroTbl(
    Type const& type )
{
    std::string tableName;
    switch( type )
    {
        case WIND:
        {
            tableName = "wind";
            static bool ptr( CreateErosionTable( tableName ) );
            break;
        }
        case WATER:
        {
            tableName = "water";
            static bool ptr( CreateErosionTable( tableName ) );
            break;
        }
    }

    return tableName;
}
////////////////////////////////////////////////////////////////////////////////
std::string EroByCrpTbl(
    Type const& type )
{
    std::string tableName;
    switch( type )
    {
        case WIND:
        {
            tableName = "wind_bycrop";
            static bool ptr( CreateErosionByCropTable( tableName, type ) );
            break;
        }
        case WATER:
        {
            tableName = "water_bycrop";
            static bool ptr( CreateErosionByCropTable( tableName, type ) );
            break;
        }
    }

    return tableName;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateErosionTable(
    std::string const& tableName )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "create table if not exists " << tableName.c_str() << "(\n"
        << "  scenario_id char( 36 ) not null,\n"
        << "  sci float not null,\n"
        << "  sciom float not null,\n"
        << "  scier float not null,\n"
        << "  scifo float not null,\n"
        << "  winderos float not null,\n"
        << "  watereros float not null,\n"
        << "  totbiorem float not null,\n"
        << "constraint " << tableName.c_str() << "_pkey\n"
        << "  primary key(\n"
        << "    scenario_id ),\n"
        << "constraint " << tableName.c_str() << "_fkey\n"
        << "  foreign key(\n"
        << "    scenario_id )\n"
        << "  references scenario(\n"
        << "    id )\n"
        << "  on delete cascade );";
    ExecuteRetry( stmtObj );

    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateErosionByCropTable(
    std::string const& tableName,
    Type const& type )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "create table if not exists " << tableName.c_str() << "(\n"
        << "  scenario_id char( 36 ) not null,\n"
        << "  crop varchar( 50 ) not null,\n"
        << "  totbiorem float not null,\n"
        << "  rnk integer not null,\n"
        << "constraint " << tableName.c_str() << "_ukey\n"
        << "  unique(\n"
        << "    scenario_id,\n"
        << "    rnk ),\n"
        << "constraint " << tableName.c_str() << "_fkey\n"
        << "  foreign key(\n"
        << "    scenario_id )\n"
        << "  references " << EroTbl( type ) << "(\n"
        << "    scenario_id )\n"
        << "  on delete cascade );";
    ExecuteRetry( stmtObj );

    return true;
}
////////////////////////////////////////////////////////////////////////////////

} //end erosion
} //end open
} //end leaf
