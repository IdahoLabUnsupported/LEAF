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
#include <leaf/wrapper/weru/Harvest.h>

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
Harvest::Harvest()
    :
    Object(),
    m_scenarioId(),
    m_crop(),
    m_cropIndex(),
    m_cycleNumber(),
    m_dryYield(),
    m_harvestDate(),
    m_index(),
    m_moisture(),
    m_residue(),
    m_runId(),
    m_yield(),
    m_yieldUnits()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Harvest::Harvest(
    scn::ScenarioPtr const& scenario )
    :
    Object(),
    m_scenarioId( scenario->GetId() ),
    m_crop(),
    m_cropIndex(),
    m_cycleNumber(),
    m_dryYield(),
    m_harvestDate(),
    m_index(),
    m_moisture(),
    m_residue(),
    m_runId(),
    m_yield(),
    m_yieldUnits()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Harvest::~Harvest()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
HarvestPtr Harvest::Create()
{
    return boost::make_shared< Harvest >();
}
////////////////////////////////////////////////////////////////////////////////
HarvestPtr Harvest::Create(
    scn::ScenarioPtr const& scenario )
{
    return boost::make_shared< Harvest >( scenario );
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetScenarioId() const
{
    return m_scenarioId;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetCrop() const
{
    return m_crop;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetCropIndex() const
{
    return m_cropIndex;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetCycleNumber() const
{
    return m_cycleNumber;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetDryYield() const
{
    return m_dryYield;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetHarvestDate() const
{
    return m_harvestDate;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetIndex() const
{
    return m_index;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetMoisture() const
{
    return m_moisture;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetResidue() const
{
    return m_residue;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetRunId() const
{
    return m_runId;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetYield() const
{
    return m_yield;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Harvest::GetYieldUnits() const
{
    return m_yieldUnits;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetCrop(
    Poco::Dynamic::Var const& crop )
{
    m_crop = crop;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetCropIndex(
    Poco::Dynamic::Var const& cropIndex )
{
    m_cropIndex = cropIndex;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetCycleNumber(
    Poco::Dynamic::Var const& cycleNumber )
{
    m_cycleNumber = cycleNumber;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetDryYield(
    Poco::Dynamic::Var const& dryYield )
{
    m_dryYield = dryYield;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetHarvestDate(
    Poco::Dynamic::Var const& harvestDate )
{
    m_harvestDate = harvestDate;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetIndex(
    Poco::Dynamic::Var const& index )
{
    m_index = index;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetMoisture(
    Poco::Dynamic::Var const& moisture )
{
    m_moisture = moisture;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetResidue(
    Poco::Dynamic::Var const& residue )
{
    m_residue = residue;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetRunId(
    Poco::Dynamic::Var const& runId )
{
    m_runId = runId;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetYield(
    Poco::Dynamic::Var const& yield )
{
    m_yield = yield;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::SetYieldUnits(
    Poco::Dynamic::Var const& yieldUnits )
{
    m_yieldUnits = yieldUnits;
}
////////////////////////////////////////////////////////////////////////////////
void Harvest::WriteToDB()
{
    StmtObj stmtObj( GetSession( DB_LEAF ) );
    stmtObj.m_statement
        << "insert into " << HrvTbl() << "\n"
        << "values(\n"
        << "  ? /*scenario_id*/,\n"
        << "  ? /*crop*/,\n"
        << "  ? /*cropindex*/,\n"
        << "  ? /*cyclenumber*/,\n"
        << "  ? /*dryyield*/,\n"
        << "  ? /*harvestdate*/,\n"
        << "  ? /*index*/,\n"
        << "  ? /*moisture*/,\n"
        << "  ? /*residue*/,\n"
        << "  ? /*runid*/,\n"
        << "  ? /*yield*/,\n"
        << "  ? /*yieldunits*/ )",
        useRef( *this );
    ExecuteRetry( stmtObj );
}
////////////////////////////////////////////////////////////////////////////////

} //end weru
} //end wrapper
} //end leaf
