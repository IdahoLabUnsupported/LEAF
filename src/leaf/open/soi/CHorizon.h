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
#include <leaf/open/soi/CHorizonPtr.h>
#include <leaf/open/soi/ChFrag.h>
#include <leaf/open/soi/ChTextureGrp.h>

#ifndef SWIG
struct CHKEY{};
struct HZDEPT{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS CHorizon : public Object
{
public:
    ///
    virtual ~CHorizon();

    ///
    ChFragPtr GetFrag(
        std::string const& chfragskey,
        bool const& mustExist = false ) const;

#ifndef SWIG
    ///
    ChFrags const& GetFrags() const;
#endif //SWIG

    ///
    std::size_t GetNumFrags() const;

    ///
    ChTextureGrpPtr GetTextureGrp(
        std::string const& chtgkey,
        bool const& mustExist = false ) const;

#ifndef SWIG
    ///
    ChTextureGrps const& GetTextureGrps() const;
#endif //SWIG

    ///
    std::size_t GetNumTextureGrps() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetCaco3(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetCec7(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Dynamic::Var const& GetChkey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetClaytotal(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Dynamic::Var const& GetCokey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetDbthirdbar(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetDesgnmaster() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetEcec(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetHzdepb(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///For boost::multi_index
    Poco::Nullable< Poco::Dynamic::Var > const& GetHzdept_r() const;
    ///For everyone else
    Poco::Nullable< Poco::Dynamic::Var > const& GetHzdept(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetHzname() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetHzthk(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetKffact() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetKsat(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetLep(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetOm(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetPh01mcacl2(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetPh1to1h2o(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandco(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandfine(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandmed(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandtotal(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandvc(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSandvf(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSilttotal(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetWfifteenbar(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetWtenthbar(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetWthirdbar(
        unsigned int const& valType = SsurgoValType::R ) const;

    ///
    bool const& HasFrags() const;

    ///
    bool const& HasTextureGrps() const;

    ///
    void Populate() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        CHorizonPtr const& chorizon );
#endif //SWIG

protected:
    ///
    CHorizon();

private:
    ///
    friend class Component;
    friend class Poco::Data::TypeHandler< CHorizon >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< CHorizon >(
        void*, CHorizon const& );
    friend CHorizonPtr boost::make_shared< CHorizon >();

    ///
    mutable boost::once_flag m_of1;
    void PopulateFrags() const;

    ///
    mutable boost::once_flag m_of2;
    void PopulateTextureGrps() const;

    ///
    static std::string const INPUT;

    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_caco3;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_cec7;

    //boost multi_index key
    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_chkey;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_claytotal;

    ///
    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_cokey;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_dbthirdbar;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_desgnmaster;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_ecec;

    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_hzdepb;

    //boost multi_index key
    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_hzdept;

    ///varchar( 12 )
    Poco::Nullable< Poco::Dynamic::Var > m_hzname;

    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_hzthk;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_kffact;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_ksat;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_lep;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_om;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_ph01mcacl2;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_ph1to1h2o;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandco;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandfine;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandmed;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandtotal;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandvc;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_sandvf;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_silttotal;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_wfifteenbar;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_wtenthbar;

    ///real
    SSURGO_VEC< Poco::Dynamic::Var > m_wthirdbar;

    ///
    mutable bool m_hasFrags;

    ///
    mutable bool m_hasTextureGrps;

    ///
    mutable ChFrags m_frags;

    ///
    mutable ChTextureGrps m_textureGrps;

};

#ifndef SWIG
///Define a multiply indexed set with indices by chkey and hzdept_r
typedef boost::multi_index_container<
    CHorizonPtr,
    boost::multi_index::indexed_by<
        //Sort by chkey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CHKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( CHorizon,
                Poco::Dynamic::Var const&, GetChkey ),
            util::less_than< std::string >
        >,

        //Sort by hzdept_r
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag< HZDEPT >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( CHorizon,
                Poco::Nullable< Poco::Dynamic::Var > const&, GetHzdept_r ),
            util::less_than< int >
        >
    >
> CHorizons;

///
typedef CHorizons::index< CHKEY >::type CHorizonsByChkey;
typedef CHorizons::index< HZDEPT >::type CHorizonsByHzdept;
typedef std::pair<
    CHorizonsByHzdept::const_iterator,
    CHorizonsByHzdept::const_iterator > CHorizonsByHzdeptRange;
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
class TypeHandler< class leaf::open::soi::CHorizon >
{
public:
    ///
    static std::size_t size()
    {
        //We handle seventy-four columns of the Table
        return 74;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::soi::CHorizon const& obj,
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
        leaf::open::soi::CHorizon& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::CHorizon& obj,
        leaf::open::soi::CHorizon const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //caco3
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_caco3.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_caco3.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_caco3.H(), Keywords::null, pE );
        obj.m_caco3.FixUpR();
        //cec7
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_cec7.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_cec7.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_cec7.H(), Keywords::null, pE );
        obj.m_cec7.FixUpR();
        //chkey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_chkey, defVal.m_chkey, pE );
        //claytotal
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_claytotal.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_claytotal.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_claytotal.H(), Keywords::null, pE );
        obj.m_claytotal.FixUpR();
        //cokey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_cokey, defVal.m_cokey, pE );
        //dbthirdbar
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_dbthirdbar.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_dbthirdbar.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_dbthirdbar.H(), Keywords::null, pE );
        obj.m_dbthirdbar.FixUpR();
        //desgnmaster
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_desgnmaster, Keywords::null, pE );
        //ecec
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ecec.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ecec.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ecec.H(), Keywords::null, pE );
        obj.m_ecec.FixUpR();
        //hzdepb
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdepb.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdepb.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdepb.H(), Keywords::null, pE );
        obj.m_hzdepb.FixUpR();
        //hzdept
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdept.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdept.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzdept.H(), Keywords::null, pE );
        obj.m_hzdept.FixUpR();
        //hzname
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzname, Keywords::null, pE );
        //hzthk
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzthk.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzthk.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_hzthk.H(), Keywords::null, pE );
        obj.m_hzthk.FixUpR();
        //kffact
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_kffact, Keywords::null, pE );
        //ksat
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ksat.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ksat.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ksat.H(), Keywords::null, pE );
        obj.m_ksat.FixUpR();
        //lep
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_lep.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_lep.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_lep.H(), Keywords::null, pE );
        obj.m_lep.FixUpR();
        //om
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_om.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_om.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_om.H(), Keywords::null, pE );
        obj.m_om.FixUpR();
        //ph01mcacl2
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph01mcacl2.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph01mcacl2.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph01mcacl2.H(), Keywords::null, pE );
        obj.m_ph01mcacl2.FixUpR();
        //ph1to1h2o
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph1to1h2o.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph1to1h2o.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_ph1to1h2o.H(), Keywords::null, pE );
        obj.m_ph1to1h2o.FixUpR();
        //sandco
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandco.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandco.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandco.H(), Keywords::null, pE );
        obj.m_sandco.FixUpR();
        //sandfine
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandfine.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandfine.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandfine.H(), Keywords::null, pE );
        obj.m_sandfine.FixUpR();
        //sandmed
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandmed.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandmed.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandmed.H(), Keywords::null, pE );
        obj.m_sandmed.FixUpR();
        //sandtotal
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandtotal.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandtotal.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandtotal.H(), Keywords::null, pE );
        obj.m_sandtotal.FixUpR();
        //sandvc
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvc.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvc.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvc.H(), Keywords::null, pE );
        obj.m_sandvc.FixUpR();
        //sandvf
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvf.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvf.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_sandvf.H(), Keywords::null, pE );
        obj.m_sandvf.FixUpR();
        //silttotal
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_silttotal.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_silttotal.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_silttotal.H(), Keywords::null, pE );
        obj.m_silttotal.FixUpR();
        //wfifteenbar
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wfifteenbar.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wfifteenbar.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wfifteenbar.H(), Keywords::null, pE );
        obj.m_wfifteenbar.FixUpR();
        //wtenthbar
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wtenthbar.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wtenthbar.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wtenthbar.H(), Keywords::null, pE );
        obj.m_wtenthbar.FixUpR();
        //wthirdbar
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wthirdbar.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wthirdbar.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_wthirdbar.H(), Keywords::null, pE );
        obj.m_wthirdbar.FixUpR();
    }

};
#endif //SWIG

} //end Data
} //end Poco
