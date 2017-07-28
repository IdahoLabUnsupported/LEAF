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


#pragma once

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>

#include <leaf/open/Object.h>

#include <leaf/wrapper/weru/Common.h>
#include <leaf/wrapper/weru/HarvestPtr.h>

#include <leaf/open/scn/Scenario.h>

#ifndef SWIG
struct HRVKEY{};
#endif //SWIG

namespace leaf
{
namespace wrapper
{
namespace weru
{

class LEAF_WEPS_EXPORTS Harvest : public open::Object
{
public:
    ///
    virtual ~Harvest();

    ///
    static HarvestPtr Create(
        open::scn::ScenarioPtr const& scenario );

    ///
    Poco::Dynamic::Var const& GetScenarioId() const;

    ///
    Poco::Dynamic::Var const& GetCrop() const;

    ///
    Poco::Dynamic::Var const& GetCropIndex() const;

    ///
    Poco::Dynamic::Var const& GetCycleNumber() const;

    ///
    Poco::Dynamic::Var const& GetDryYield() const;

    ///
    Poco::Dynamic::Var const& GetHarvestDate() const;

    ///
    Poco::Dynamic::Var const& GetIndex() const;

    ///
    Poco::Dynamic::Var const& GetMoisture() const;

    ///
    Poco::Dynamic::Var const& GetResidue() const;

    ///
    Poco::Dynamic::Var const& GetRunId() const;

    ///
    Poco::Dynamic::Var const& GetYield() const;

    ///
    Poco::Dynamic::Var const& GetYieldUnits() const;

    ///
    void SetCrop(
        Poco::Dynamic::Var const& crop );

    ///
    void SetCropIndex(
        Poco::Dynamic::Var const& cropIndex );

    ///
    void SetCycleNumber(
        Poco::Dynamic::Var const& cycleNumber );

    ///
    void SetDryYield(
        Poco::Dynamic::Var const& dryYield );

    ///
    void SetHarvestDate(
        Poco::Dynamic::Var const& harvestDate );

    ///
    void SetIndex(
        Poco::Dynamic::Var const& index );

    ///
    void SetMoisture(
        Poco::Dynamic::Var const& moisture );

    ///
    void SetResidue(
        Poco::Dynamic::Var const& residue );

    ///
    void SetRunId(
        Poco::Dynamic::Var const& runId );

    ///
    void SetYield(
        Poco::Dynamic::Var const& yield );

    ///
    void SetYieldUnits(
        Poco::Dynamic::Var const& yieldUnits );

    ///
    void WriteToDB();

#ifndef SWIG
    ///
    LEAF_WEPS_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        HarvestPtr const& harvest );
#endif //SWIG

protected:
    ///
    Harvest();

    ///
    Harvest(
        open::scn::ScenarioPtr const& scenario );

    ///
    static HarvestPtr Create();

private:
    ///
    friend class Poco::Data::TypeHandler< Harvest >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Harvest >(
        void*, Harvest const& );
    friend HarvestPtr boost::make_shared< Harvest >();
    friend HarvestPtr boost::make_shared< Harvest,
        open::scn::ScenarioPtr const& >(
        open::scn::ScenarioPtr const& );

    ///
    Poco::Dynamic::Var m_scenarioId;

    ///
    Poco::Dynamic::Var m_crop;

    ///
    Poco::Dynamic::Var m_cropIndex;

    ///
    Poco::Dynamic::Var m_cycleNumber;

    ///
    Poco::Dynamic::Var m_dryYield;

    ///
    Poco::Dynamic::Var m_harvestDate;

    ///
    Poco::Dynamic::Var m_index;

    ///
    Poco::Dynamic::Var m_moisture;

    ///
    Poco::Dynamic::Var m_residue;

    ///
    Poco::Dynamic::Var m_runId;

    ///
    Poco::Dynamic::Var m_yield;

    ///
    Poco::Dynamic::Var m_yieldUnits;

};

} //end weru
} //end wrapper
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::wrapper::weru::Harvest >
{
public:
    ///
    static std::size_t size()
    {
        //We handle twelve columns of the Table
        return 12;
    }

    ///
   static void bind(
       std::size_t pos,
       leaf::wrapper::weru::Harvest const& obj,
       AbstractBinder* pB,
       AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_scenarioId, pB, dir );

        //crop
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_crop, pB, dir );

        //cropIndex
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_cropIndex, pB, dir );

        //cycleNumber
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_cycleNumber, pB, dir );

        //dryYield
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_dryYield, pB, dir );

        //harvestDate
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_harvestDate, pB, dir );

        //index
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_index, pB, dir );

        //moisture
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_moisture, pB, dir );

        //residue
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_residue, pB, dir );

        //runId
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_runId, pB, dir );

        //yield
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_yield, pB, dir );

        //yieldUnits
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_yieldUnits, pB, dir );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::wrapper::weru::Harvest& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_scenarioId, pP );

        //crop
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_crop, pP );

        //cropIndex
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_cropIndex, pP );

        //cycleNumber
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_cycleNumber, pP );

        //dryYield
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_dryYield, pP );

        //harvestDate
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_harvestDate, pP );

        //index
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_index, pP );

        //moisture
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_moisture, pP );

        //residue
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_residue, pP );

        //runId
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_runId, pP );

        //yield
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_yield, pP );

        //yieldUnits
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_yieldUnits, pP );
    }

    ///
    static void extract(
        std::size_t pos,
        leaf::wrapper::weru::Harvest& obj,
        leaf::wrapper::weru::Harvest const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_scenarioId, defVal.m_scenarioId, pE );

        //crop
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_crop, defVal.m_crop, pE );

        //cropIndex
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_cropIndex, defVal.m_cropIndex, pE );

        //cycleNumber
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_cycleNumber, defVal.m_cycleNumber, pE );

        //dryYield
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_dryYield, defVal.m_dryYield, pE );

        //harvestDate
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_harvestDate, defVal.m_harvestDate, pE );

        //index
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_index, defVal.m_index, pE );

        //moisture
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_moisture, defVal.m_moisture, pE );

        //residue
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_residue, defVal.m_residue, pE );

        //runId
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_runId, defVal.m_runId, pE );

        //yield
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_yield, defVal.m_yield, pE );

        //yieldUnits
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_yieldUnits, defVal.m_yieldUnits, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
