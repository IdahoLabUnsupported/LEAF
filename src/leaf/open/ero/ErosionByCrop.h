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
#include <leaf/open/ero/ErosionByCropPtr.h>

namespace leaf
{
namespace open
{
namespace ero
{

///
typedef std::vector< ErosionByCropPtr > ErosionByCrops;

class LEAF_OPEN_EXPORTS ErosionByCrop : public Object
{
public:
    ///
    virtual ~ErosionByCrop();

    static ErosionByCropPtr Create(
        ErosionPtr const& erosion,
        std::string const& crop,
        double const& totBioRem,
        unsigned int const& rnk );

    ///
    Type const& GetType() const;

    ///
    Poco::Dynamic::Var const& GetScenarioId() const;

    ///
    Poco::Dynamic::Var const& GetCrop() const;

    ///
    Poco::Dynamic::Var const& GetTotBioRem() const;

    ///
    Poco::Dynamic::Var const& GetRnk() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ErosionByCropPtr const& eroByCrop );
#endif //SWIG

protected:
    ///
    ErosionByCrop();

    ///
    ErosionByCrop(
        ErosionPtr const& erosion,
        std::string const& crop,
        double const& totBioRem,
        unsigned int const& rnk );

private:
    ///
    friend class Erosion;
    friend class Poco::Data::TypeHandler< ErosionByCrop >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< ErosionByCrop >(
        void*, ErosionByCrop const& );
    friend ErosionByCropPtr boost::make_shared< ErosionByCrop >();
    friend ErosionByCropPtr boost::make_shared< ErosionByCrop,
        ErosionPtr const&,
        std::string const&,
        double const&,
        unsigned int const& >(
        ErosionPtr const&,
        std::string const&,
        double const&,
        unsigned int const& );

    ///
    static ErosionByCrops Extract(
        ErosionPtr const& erosion );

    ///Write data to erosion by crop database
    void WriteToDB();

    ///
    Type m_type;

    ///
    Poco::Dynamic::Var m_scenarioId;

    ///
    ///Weps -
    ///Rusle2 - SEG_SIM_DAY_TOTAL_BIOMASS_REMOVAL_RATE
    Poco::Dynamic::Var m_crop;

    ///
    ///Weps -
    ///Rusle2 - SEG_SIM_DAY_TOTAL_BIOMASS_REMOVAL_RATE
    Poco::Dynamic::Var m_totBioRem;

    ///
    Poco::Dynamic::Var m_rnk;

};

} //end erosion
} //end open
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::open::ero::ErosionByCrop >
{
public:
    ///
    static std::size_t size()
    {
        //We handle four columns of the Table
        return 4;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::ero::ErosionByCrop const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_scenarioId, pB, dir );
        //crop
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_crop, pB, dir );
        //totBioRem
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_totBioRem, pB, dir );
        //rnk
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_rnk, pB, dir );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::open::ero::ErosionByCrop& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_scenarioId, pP );
        //crop
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_crop, pP );
        //totBioRem
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_totBioRem, pP );
        //rnk
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_rnk, pP );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::ero::ErosionByCrop& obj,
        leaf::open::ero::ErosionByCrop const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != 0 );

        //scenarioId
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_scenarioId, defVal.m_scenarioId, pE );
        //crop
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_crop, defVal.m_crop, pE );
        //totBioRem
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_totBioRem, defVal.m_totBioRem, pE );
        //rnk
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_rnk, defVal.m_rnk, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
