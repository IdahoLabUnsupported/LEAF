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
#include <leaf/wrapper/weru/CalibrationPtr.h>

#include <leaf/open/scn/Scenario.h>

#ifndef SWIG
struct CALIBKEY{};
#endif //SWIG

namespace leaf
{
namespace wrapper
{
namespace weru
{

class LEAF_WEPS_EXPORTS Calibration : public open::Object
{
public:
    ///
    virtual ~Calibration();

    ///
    static CalibrationPtr Create(
        open::scn::ScenarioPtr const& scenario,
        Poco::Dynamic::Var const& date,
        Poco::Dynamic::Var const& crop,
        Poco::Dynamic::Var const& bioAdjFac );

    ///
    Poco::Dynamic::Var const& GetScenarioId() const;

    ///
    Poco::Dynamic::Var const& GetTgtYld() const;

    ///
    Poco::Dynamic::Var const& GetDate() const;

    ///
    Poco::Dynamic::Var const& GetCrop() const;

    ///
    Poco::Dynamic::Var const& GetBioAdjFactor() const;

    ///
    void SetTgtYld(
        Poco::Dynamic::Var const& tgtYld );

    ///
    void WriteToDB();

#ifndef SWIG
    ///
    LEAF_WEPS_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        CalibrationPtr const& calibration );
#endif //SWIG

protected:
    ///
    Calibration();

    ///
    Calibration(
        open::scn::ScenarioPtr const& scenario,
        Poco::Dynamic::Var const& date,
        Poco::Dynamic::Var const& crop,
        Poco::Dynamic::Var const& bioAdjFac );

    ///
    static CalibrationPtr Create();

private:
    ///
    friend class Poco::Data::TypeHandler< Calibration >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Calibration >(
        void*, Calibration const& );
    friend CalibrationPtr boost::make_shared< Calibration >();
    friend CalibrationPtr boost::make_shared< Calibration,
        open::scn::ScenarioPtr const&,
        Poco::Dynamic::Var const&,
        Poco::Dynamic::Var const&,
        Poco::Dynamic::Var const& >(
        open::scn::ScenarioPtr const&,
        Poco::Dynamic::Var const&,
        Poco::Dynamic::Var const&,
        Poco::Dynamic::Var const& );

    ///
    Poco::Dynamic::Var m_scenarioId;

    ///
    mutable Poco::Dynamic::Var m_tgtYld;

    ///
    Poco::Dynamic::Var m_date;

    ///
    Poco::Dynamic::Var m_crop;

    ///
    Poco::Dynamic::Var m_bioAdjFac;

};

#ifndef SWIG
///Define a multiply indexed set with indices by cokey
typedef boost::multi_index_container<
    CalibrationPtr,
    boost::multi_index::indexed_by<
        //Sort by ctyfips and then mukey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CALIBKEY >,
            boost::multi_index::composite_key<
                Calibration,
                BOOST_MULTI_INDEX_CONST_MEM_FUN( Calibration,
                    Poco::Dynamic::Var const&, GetScenarioId ),
                BOOST_MULTI_INDEX_CONST_MEM_FUN( Calibration,
                    Poco::Dynamic::Var const&, GetDate ),
                BOOST_MULTI_INDEX_CONST_MEM_FUN( Calibration,
                    Poco::Dynamic::Var const&, GetCrop )
            >
        >
    >
> Calibrations;

///
typedef Calibrations::index< CALIBKEY >::type CalibsByKey;
#endif //SWIG

} //end weru
} //end wrapper
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::wrapper::weru::Calibration >
{
public:
    ///
    static std::size_t size()
    {
        //We handle five columns of the Table
        return 5;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::wrapper::weru::Calibration const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_scenarioId, pB, dir );
        //tgtYld
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_tgtYld, pB, dir );
        //date
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_date, pB, dir );
        //crop
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_crop, pB, dir );
        //bioAdjFac
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_bioAdjFac, pB, dir );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::wrapper::weru::Calibration& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_scenarioId, pP );
        //tgtYld
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_tgtYld, pP );
        //date
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_date, pP );
        //crop
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_crop, pP );
        //bioAdjFac
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_bioAdjFac, pP );
    }

    ///
    static void extract(
        std::size_t pos,
        leaf::wrapper::weru::Calibration& obj,
        leaf::wrapper::weru::Calibration const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //scenarioId
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_scenarioId, defVal.m_scenarioId, pE );
        //tgtYld
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_tgtYld, defVal.m_tgtYld, pE );
        //date
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_date, defVal.m_date, pE );
        //crop
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_crop, defVal.m_crop, pE );
        //bioAdjFac
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_bioAdjFac, defVal.m_bioAdjFac, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
