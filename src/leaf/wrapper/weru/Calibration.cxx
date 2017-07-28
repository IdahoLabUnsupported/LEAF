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
#include <leaf/wrapper/weru/Calibration.h>

using namespace leaf::open;
using namespace leaf::util;

// --- Boost Includes --- //
#include <boost/math/special_functions/round.hpp>

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace wrapper
{
namespace weru
{

////////////////////////////////////////////////////////////////////////////////
Calibration::Calibration()
    :
    Object(),
    m_scenarioId(),
    m_tgtYld(),
    m_date(),
    m_crop(),
    m_bioAdjFac()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Calibration::Calibration(
    scn::ScenarioPtr const& scenario,
    Var const& date,
    Var const& crop,
    Var const& bioAdjFac )
    :
    Object(),
    m_scenarioId( scenario->GetId() ),
    m_tgtYld(),
    m_date( date ),
    m_crop( crop ),
    m_bioAdjFac( bioAdjFac )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Calibration::~Calibration()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CalibrationPtr Calibration::Create()
{
    return boost::make_shared< Calibration >();
}
////////////////////////////////////////////////////////////////////////////////
CalibrationPtr Calibration::Create(
    scn::ScenarioPtr const& scenario,
    Var const& date,
    Var const& crop,
    Var const& bioAdjFac )
{
    return boost::make_shared< Calibration >( scenario, date, crop, bioAdjFac );
}
////////////////////////////////////////////////////////////////////////////////
Var const& Calibration::GetScenarioId() const
{
    return m_scenarioId;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Calibration::GetTgtYld() const
{
    return m_tgtYld;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Calibration::GetDate() const
{
    return m_date;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Calibration::GetCrop() const
{
    return m_crop;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Calibration::GetBioAdjFactor() const
{
    return m_bioAdjFac;
}
////////////////////////////////////////////////////////////////////////////////
void Calibration::SetTgtYld(
    Var const& tgtYld )
{
    //Round the target yield to nearest whole integer
    m_tgtYld = boost::math::iround( Convert< double >( tgtYld ) );
}
////////////////////////////////////////////////////////////////////////////////
/*bool Calibration::Exists(
    Var const& name,
    Var const& objid,
    Var const& compval,
    Var const& rotation,
    Var const& tillreg,
    Var const& tgtyield,
    Var const& date,
    Var const& crop )
{
    //Round the yield before we query
    unsigned int yield = boost::math::iround( tgtyield.convert< double >() );
    unsigned int cnt( 0 );

    StmtObj stmtObj( CreateSession() );
    stmtObj.m_statement
        << "select\n"
        << "  count( * )\n"
        << "from [" << CalAdjTbl() << "]\n"
        << "where\n"
        << "  name = :name and\n"
        << "  objid = :objid and\n"
        << "  compval = :compval and\n"
        << "  rotation = :rotation and\n"
        << "  tillreg = :tillreg and\n"
        << "  tgtyield = :tgtyield and\n"
        << "  date = :date and\n"
        << "  crop = :crop",
        into( cnt ),
        useRef( name ),
        useRef( objid ),
        useRef( compval ),
        useRef( rotation ),
        useRef( tillreg ),
        useRef( yield ),
        useRef( date ),
        useRef( crop );
    ExecuteRetry( stmtObj );

    return ( cnt != 0 );
}
////////////////////////////////////////////////////////////////////////////////
Var Calibration::GetBioAdjFactor(
    Var const& name,
    Var const& objid,
    Var const& compval,
    Var const& rotation,
    Var const& tillreg,
    Var const& tgtyield,
    Var const& date,
    Var const& crop )
{
    Var bioAdjFac;

    StmtObj stmtObj( CreateSession() );
    stmtObj.m_statement
        << "select\n"
        << "  t1.value\n"
        << "from [" << CalAdjTbl() << "] as t1\n"
        << "where\n"
        << "  t1.name = :name and\n"
        << "  t1.objid = :objid and\n"
        << "  t1.compval = :compval and\n"
        << "  t1.rotation = :rotation and\n"
        << "  t1.tillreg = :tillreg and\n"
        << "  t1.date = :date and\n"
        << "  t1.crop = :crop\n"
        << "order by\n"
        << "  abs( t1.tgtyield - :tgtyield ) asc,\n"
        << "  t1.tgtyield desc\n"
        << "limit 1",
        into( bioAdjFac ),
        useRef( name ),
        useRef( objid ),
        useRef( compval ),
        useRef( rotation ),
        useRef( tillreg ),
        useRef( date ),
        useRef( crop ),
        useRef( tgtyield );
    ExecuteRetry( stmtObj );

    assert( !( bioAdjFac.isEmpty() ) );
    return bioAdjFac;
}*/
////////////////////////////////////////////////////////////////////////////////
void Calibration::WriteToDB()
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "insert into " << CalTbl() << "\n"
        << "values(\n"
        << "  ? /*scenario_id*/,\n"
        << "  ? /*tgtyld*/,\n"
        << "  ? /*date*/,\n"
        << "  ? /*crop*/,\n"
        << "  ? /*bioadjfac*/ )",
        useRef( *this );
    ExecuteRetry( stmtObj );
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    CalibrationPtr const& calibration )
{
    os
        << "WEPS Calibration: " << &*calibration
        << std::endl
        << "  scenario_id: "
        << Convert< std::string >( calibration->GetScenarioId() )
        << std::endl
        << "  tgtyld: "
        << Convert< std::string >( calibration->GetTgtYld() )
        << std::endl
        << "  date: "
        << Convert< std::string >( calibration->GetDate() )
        << std::endl
        << "  crop: "
        << Convert< std::string >( calibration->GetCrop() )
        << std::endl
        << "  bioadjfac: "
        << Convert< std::string >( calibration->GetBioAdjFactor() )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end weru
} //end wrapper
} //end leaf
