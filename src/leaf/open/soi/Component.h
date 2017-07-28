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

#include <leaf/open/soi/Common.h>
#include <leaf/open/soi/ComponentPtr.h>
#include <leaf/open/soi/Corestriction.h>
#include <leaf/open/soi/CHorizon.h>

#ifndef SWIG
struct COKEY{};
struct COMPPCT{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class MapUnit;

class LEAF_OPEN_EXPORTS Component : public Object
{
public:
    ///
    virtual ~Component();

    ///
    CorestrictionPtr GetCorestriction(
        std::string const& corestrictkey,
        bool const& mustExist = false ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > GetMinResDept() const;

#ifndef SWIG
    ///
    Corestrictions const& GetCorestrictions() const;
#endif //SWIG

    ///
    std::size_t GetNumCorestrictions() const;

    ///
    CHorizonPtr GetHorizon(
        std::string const& chkey,
        bool const& mustExist = false ) const;

#ifndef SWIG
    ///
    CHorizons const& GetHorizons() const;

    ///
    CHorizonsByHzdeptRange GetHorizonsByHzdeptRange(
        unsigned int const& lowerBound = 0,
        unsigned int const& upperBound = UINT_MAX ) const;
#endif //SWIG

    ///
    std::size_t GetNumHorizons() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetAlbedodry(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Dynamic::Var const& GetCokey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetCompkind() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetCompname() const;

    ///For boost::multi_index
    Poco::Nullable< Poco::Dynamic::Var > const& GetComppct_r() const;
    ///For everyone else
    Poco::Nullable< Poco::Dynamic::Var > const& GetComppct(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetGeomdesc() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetHydgrp() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetLocalphase() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetMajcompflag() const;

    ///
    Poco::Dynamic::Var const& GetMukey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetNirrcapcl() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSlope(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSlopelenusle(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetTaxorder() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetTaxsubgrp() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetTfact() const;

    ///
    MapUnit const& GetMapUnit() const;

    ///
    bool const& HasCorestrictions() const;

    ///
    bool const& HasHorizons() const;

    ///
    void Populate() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ComponentPtr const& component );
#endif //SWIG

protected:
    ///
    Component();

private:
    ///
    friend class MapUnit;
    friend class Poco::Data::TypeHandler< Component >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Component >(
        void*, Component const& );
    friend ComponentPtr boost::make_shared< Component >();

    ///
    mutable boost::once_flag m_of1;
    void PopulateCorestrictions() const;

    ///
    mutable boost::once_flag m_of2;
    void PopulateHorizons() const;

    ///
    static std::string const INPUT;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_albedodry;

    //boost multi_index key
    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_cokey;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_compkind;

    ///varchar( 60 )
    Poco::Nullable< Poco::Dynamic::Var > m_compname;

    //boost multi_index key
    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_comppct;

    ///text
    Poco::Nullable< Poco::Dynamic::Var > m_geomdesc;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_hydgrp;

    ///varchar( 40 )
    Poco::Nullable< Poco::Dynamic::Var > m_localphase;

    ///char( 3 )
    Poco::Nullable< Poco::Dynamic::Var > m_majcompflag;

    ///varchar(30) not null
    Poco::Dynamic::Var m_mukey;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_nirrcapcl;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_slope;

    ///smallint - meters
    SSURGO_VEC< Poco::Dynamic::Var > m_slopelenusle;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_taxorder;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_taxsubgrp;

    ///smallint
    Poco::Nullable< Poco::Dynamic::Var > m_tfact;

    ///Need to be careful with this
    ///Soil hierarchy is constructed on the stack and should switch to use pts
    mutable MapUnit const* m_mapUnit;

    ///
    mutable bool m_hasCorestrictions;

    ///
    mutable bool m_hasHorizons;

    ///
    mutable Corestrictions m_corestrictions;

    ///
    mutable CHorizons m_horizons;

};

#ifndef SWIG
///Define a multiply indexed set with indices by cokey
typedef boost::multi_index_container<
    ComponentPtr,
    boost::multi_index::indexed_by<
        //Sort by cokey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< COKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( Component,
                Poco::Dynamic::Var const&, GetCokey ),
            util::less_than< std::string >
        >,

        //Sort by comppct_r
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag< COMPPCT >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( Component,
                Poco::Nullable< Poco::Dynamic::Var > const&, GetComppct_r ),
            util::greater_than< int >
        >
    >
> Components;

///
typedef Components::index< COKEY >::type ComponentsByCokey;
typedef Components::index< COMPPCT >::type ComponentsByComppct;
typedef std::pair<
    ComponentsByComppct::const_iterator,
    ComponentsByComppct::const_iterator > ComponentsByComppctRange;
#endif //SWIG

} //end soi
} //end open
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::open::soi::Component >
{
public:
    ///
    static std::size_t size()
    {
        //We handle twenty-four columns of the Table
        return 24;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::soi::Component const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::open::soi::Component& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::Component& obj,
        leaf::open::soi::Component const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //albedodry
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_albedodry.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_albedodry.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_albedodry.H(), Keywords::null, pE );
        obj.m_albedodry.FixUpR();
        //cokey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_cokey, defVal.m_cokey, pE );
        //compkind
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_compkind, Keywords::null, pE );
        //compname
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_compname, Keywords::null, pE );
        //comppct
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_comppct.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_comppct.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_comppct.H(), Keywords::null, pE );
        obj.m_comppct.FixUpR();
        //geomdesc
        TypeHandler< Nullable< Dynamic::Var > >::extract(
            pos++, obj.m_geomdesc, Keywords::null, pE );
        //hydgrp
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hydgrp, Keywords::null, pE );
        //localphase
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_localphase, Keywords::null, pE );
        //majcompflag
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_majcompflag, Keywords::null, pE );
        //mukey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_mukey, defVal.m_mukey, pE );
        //nirrcapcl
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_nirrcapcl, Keywords::null, pE );
        //slope
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slope.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slope.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slope.H(), Keywords::null, pE );
        obj.m_slope.FixUpR();
        //slopelenusle
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slopelenusle.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slopelenusle.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_slopelenusle.H(), Keywords::null, pE );
        obj.m_slopelenusle.FixUpR();
        //taxorder
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_taxorder, Keywords::null, pE );
        //taxsubgrp
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_taxsubgrp, Keywords::null, pE );
        //tfact
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_tfact, Keywords::null, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
