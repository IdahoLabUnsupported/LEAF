// Copyright 2017 Battelle Energy Alliance, LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// --- LEAF Includes --- //
#include <leaf/wrapper/weru/Common.h>

using namespace leaf::open;
using namespace leaf::util;

// --- POCO Includes --- //
using namespace Poco::Data;
using namespace Keywords;

namespace leaf
{
namespace wrapper
{
namespace weru
{

////////////////////////////////////////////////////////////////////////////////
std::string CalTbl()
{
    static std::string tableName = "calib";
    static bool ptr( CreateCalTbl( tableName ) );

    return tableName;
}
////////////////////////////////////////////////////////////////////////////////
std::string HrvTbl()
{
    static std::string tableName = "harvest";
    static bool ptr( CreateHrvTbl( tableName ) );

    return tableName;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateCalTbl(
    std::string const& tableName )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "create table if not exists " << tableName.c_str() << "(\n"
        << "  scenario_id char( 36 ) not null,\n"
        << "  tgtyld integer not null,\n"
        << "  date varchar( 50 ),\n"
        << "  crop varchar( 50 ) not null,\n"
        << "  bioadjfac float not null,\n"
        << "constraint " << tableName.c_str() << "_ukey\n"
        << "  unique(\n"
        << "    scenario_id,\n"
        << "    tgtyld,\n"
        << "    date,\n"
        << "    crop ),\n"
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
bool CreateHrvTbl(
    std::string const& tableName )
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "create table if not exists " << tableName.c_str() << "(\n"
        << "  scenario_id char( 36 ) not null,\n"
        << "  crop varchar( 50 ) not null,\n"
        << "  cropindex integer not null,\n"
        << "  cyclenumber integer not null,\n"
        << "  dryyield float not null,\n"
        << "  harvestdate varchar( 50 ) not null,\n"
        << "  \"index\" float not null,\n"
        << "  moisture float not null,\n"
        << "  residue float not null,\n"
        << "  runid integer not null,\n"
        << "  yield float not null,\n"
        << "  yieldunits varchar( 50 ) not null,\n"
        << "constraint " << tableName.c_str() << "_ukey\n"
        << "  unique(\n"
        << "    scenario_id,\n"
        << "    cyclenumber,\n"
        << "    cropindex ),\n"
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

} //end weru
} //end wrapper
} //end leaf
