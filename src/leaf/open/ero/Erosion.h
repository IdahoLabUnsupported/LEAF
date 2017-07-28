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


#pragma once

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>

#include <leaf/open/Object.h>

#include <leaf/open/ero/Common.h>
#include <leaf/open/ero/ErosionPtr.h>
#include <leaf/open/ero/ErosionByCrop.h>

#include <leaf/open/scn/ScenarioPtr.h>

namespace leaf
{
namespace open
{
namespace ero
{

class LEAF_OPEN_EXPORTS Erosion : public Object
{
public:
    ///
    virtual ~Erosion();

    ///
    static ErosionPtr Create(
        Type const& type,
        scn::ScenarioPtr const& scenario );

    ///
    static bool Exists(
        Type const& type,
        scn::ScenarioPtr const& scenario );

    ///
    static ErosionPtr Extract(
        Type const& type,
        scn::ScenarioPtr const& scenario );

    ///
    Type const& GetType() const;

    ///
    Poco::Dynamic::Var const& GetScenarioId() const;

    ///
    Poco::Dynamic::Var const& GetSci() const;

    ///
    Poco::Dynamic::Var const& GetSciOM() const;

    ///
    Poco::Dynamic::Var const& GetSciER() const;

    ///
    Poco::Dynamic::Var const& GetSciFO() const;

    ///
    Poco::Dynamic::Var const& GetWindEros() const;

    ///
    Poco::Dynamic::Var const& GetWaterEros() const;

    ///
    Poco::Dynamic::Var const& GetTotBioRem() const;

    ///
    void SetSci(
        Poco::Dynamic::Var const& sci );

    ///
    void SetSciOM(
        Poco::Dynamic::Var const& sciOM );

    ///
    void SetSciER(
        Poco::Dynamic::Var const& sciER );

    ///
    void SetSciFO(
        Poco::Dynamic::Var const& sciFO );

    ///
    void SetWindEros(
        Poco::Dynamic::Var const& windEros );

    ///
    void SetWaterEros(
        Poco::Dynamic::Var const& waterEros );

    ///
    void SetTotBioRem(
        Poco::Dynamic::Var const& totBioRem );

    ///Write data to erosion database
    void WriteToDB();

#ifndef SWIG
    ///
    ErosionByCropPtr operator [](
        ErosionByCrops::size_type pos ) const
    {
        return m_eroByCrops.operator []( pos );
    }

    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ErosionPtr const& erosion );
#endif //SWIG

protected:
    ///
    Erosion();

    ///
    Erosion(
        Type const& type,
        scn::ScenarioPtr const& scenario );

private:
    ///
    friend class ErosionByCrop;
    friend class Poco::Data::TypeHandler< Erosion >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Erosion >(
        void*, Erosion const& );
    friend ErosionPtr boost::make_shared< Erosion >();
    friend ErosionPtr boost::make_shared< Erosion,
        Type const&,
        scn::ScenarioPtr const& >(
        Type const&,
        scn::ScenarioPtr const& );

#ifndef SWIG
    ///
    void PushBack(
        ErosionByCropPtr const& eroByCrop );

    ///
    void SetEroByCropVec(
        ErosionByCrops const& eroByCrops );
#endif //SWIG

    ///
    Type m_type;

    ///
    Poco::Dynamic::Var m_scenarioId;

    ///Soil conditioning index (SCI)
    ///SCI = ( 0.4 * OM ) + ( 0.4 * FO ) + ( 0.2 * ER )
    ///Weps - SciEnergyResultSet::COLUMN_SCI
    ///Rusle2 - #RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_RESULT
    Poco::Dynamic::Var m_sci;

    ///SCI organic matter subfactor
    ///Weps - SciEnergyResultSet::COLUMN_OMFACTOR
    ///Rusle2 - #RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_OM_SUBFACTOR
    Poco::Dynamic::Var m_sciOM;

    ///SCI erosion subfactor
    ///Weps - SciEnergyResultSet::COLUMN_ERFACTOR
    ///Rusle2 - #RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_ER_SUBFACTOR
    Poco::Dynamic::Var m_sciER;

    ///SCI field operations (tillreg) subfactor
    ///Weps - SciEnergyResultSet::COLUMN_FOFACTOR
    ///Rusle2 - #RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_FO_SUBFACTOR
    Poco::Dynamic::Var m_sciFO;

    ///Erosion erosion soil loss (kg/m2) or (tn/ac)
    ///Weps - SciEnergyResultSet::COLUMN_WINDEROS
    ///Rusle2 - #RD:SOIL_COND_INDEX_PTR:SOIL_COND_INDEX_WIND_EROSION
    Poco::Dynamic::Var m_windEros;

    ///Erosion erosion soil loss (kg/m2) or (tn/ac)
    ///Weps - SciEnergyResultSet::COLUMN_WATEREROS
    ///Rusle2 - SLOPE_DEGRAD, SLOPE_EROD_FRAC_SOIL_LOSS,
    ///         SLOPE_DETACH, SLOPE_DELIVERY, SLOPE_SOIL_LOSS
    Poco::Dynamic::Var m_waterEros;

    ///
    ///Weps -
    ///Rusle2 - ANNUAL_TOTAL_BIOMASS_REMOVAL
    Poco::Dynamic::Var m_totBioRem;

    ///
    ///Weps -
    ///Rusle2 - SEG_SIM_DAY_TOTAL_BIOMASS_REMOVAL_RATE
    ErosionByCrops m_eroByCrops;

};

} //end ero
} //end open
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::open::ero::Erosion >
{
public:
    ///
    static std::size_t size()
    {
        //We handle eight columns of the Table
        return 8;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::ero::Erosion const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_scenarioId, pB, dir );
        //sci
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_sci, pB, dir );
        //sciOM
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_sciOM, pB, dir );
        //sciER
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_sciER, pB, dir );
        //sciFO
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_sciFO, pB, dir );
        //windEros
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_windEros, pB, dir );
        //waterEros
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_waterEros, pB, dir );
        //totBioRem
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_totBioRem, pB, dir );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::open::ero::Erosion& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_scenarioId, pP );
        //sci
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_sci, pP );
        //sciOM
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_sciOM, pP );
        //sciER
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_sciER, pP );
        //sciFO
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_sciFO, pP );
        //windEros
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_windEros, pP );
        //waterEros
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_waterEros, pP );
        //totBioRem
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_totBioRem, pP );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::ero::Erosion& obj,
        leaf::open::ero::Erosion const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_scenarioId, defVal.m_scenarioId, pE );
        //sci
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_sci, defVal.m_sci, pE );
        //sciOM
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_sciOM, defVal.m_sciOM, pE );
        //sciER
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_sciER, defVal.m_sciER, pE );
        //sciFO
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_sciFO, defVal.m_sciFO, pE );
        //windEros
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_windEros, defVal.m_windEros, pE );
        //waterEros
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_waterEros, defVal.m_waterEros, pE );
        //totBioRem
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_totBioRem, defVal.m_totBioRem, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
