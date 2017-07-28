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
#include <leaf/open/soi/MapUnitPtr.h>
#include <leaf/open/soi/Component.h>

#ifndef SWIG
struct MUKEY{};
struct MUACRES{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS MapUnit : public Object
{
public:
    ///
    virtual ~MapUnit();

    ///
    double AcresIn(
        std::string const& areatypename,
        std::string const& areasymbol ) const;

    ///
    static MapUnitPtr Create(
        std::string const& key,
        bool const& populate = true );

    ///
    static MapUnitPtr Create(
        std::string const& areatypename,
        std::string const& areasymbol,
        std::string const& musym,
        bool const& populate = true );

    ///
    ComponentPtr GetComponent(
        std::string const& cokey,
        bool const& mustExist = false ) const;

    ///
    ComponentPtr GetDominantComponent() const;

#ifndef SWIG
    ///
    Components const& GetComponents() const;
#endif //SWIG

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetAreanameLegend() const;

    ///
    Poco::Dynamic::Var const& GetAreasymbolLegend() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetBrockdepmin() const;

    ///
    Poco::Dynamic::Var const& GetLkey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetMuacres() const;

    ///
    Poco::Dynamic::Var const& GetMukey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetMuname() const;

    ///
    Poco::Dynamic::Var const& GetMusym() const;

    ///
    bool const& HasComponents() const;

    ///
    void Populate() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        MapUnitPtr const& mapUnit );
#endif //SWIG

protected:
    ///
    MapUnit();

    ///
    MapUnit(
        std::string const& key,
        bool const& populate = true );

    ///
    MapUnit(
        std::string const& areatypename,
        std::string const& areasymbol,
        std::string const& musym,
        bool const& populate = true );

private:
    ///
    friend class Soils;
    friend class Component;
    friend class Poco::Data::TypeHandler< MapUnit >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< MapUnit >(
        void*, MapUnit const& );
    friend MapUnitPtr boost::make_shared< MapUnit >();
    friend MapUnitPtr boost::make_shared< MapUnit,
        std::string const&,
        bool const& >(
        std::string const&,
        bool const& );
    friend MapUnitPtr boost::make_shared< MapUnit,
        std::string const&,
        std::string const&,
        std::string const&,
        bool const& >(
        std::string const&,
        std::string const&,
        std::string const&,
        bool const& );

    ///
    mutable boost::once_flag m_of1;
    void PopulateComponents() const;

    ///
    static std::string const INPUT;

    ///varchar( 135 )
    Poco::Nullable< Poco::Dynamic::Var > m_areaname_legend;

    ///varchar( 20 ) not null
    Poco::Dynamic::Var m_areasymbol_legend;

    ///smallint
    Poco::Nullable< Poco::Dynamic::Var > m_brockdepmin;

    ///varchar(30) not null
    Poco::Dynamic::Var m_lkey;

    ///int
    Poco::Nullable< Poco::Dynamic::Var > m_muacres;

    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_mukey;

    ///varchar( 175 )
    Poco::Nullable< Poco::Dynamic::Var > m_muname;

    ///varchar( 6 ) not null
    Poco::Dynamic::Var m_musym;

    ///
    mutable bool m_hasComponents;

    ///
    mutable Components m_components;

};

#ifndef SWIG
///Define a multiply indexed set with indices by mukey
typedef boost::multi_index_container<
    MapUnitPtr,
    boost::multi_index::indexed_by<
        //Sort by mukey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< MUKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( MapUnit,
                Poco::Dynamic::Var const&, GetMukey ),
            util::less_than< std::string >
        >,

        //Sort by muacres
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag< MUACRES >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( MapUnit,
                Poco::Nullable< Poco::Dynamic::Var > const&, GetMuacres ),
            util::greater_than< long >
        >
    >
> MapUnits;

///
typedef MapUnits::index< MUKEY >::type MapUnitsByMukey;
typedef MapUnits::index< MUACRES >::type MapUnitsByMuacres;
typedef std::pair<
    MapUnitsByMuacres::const_iterator,
    MapUnitsByMuacres::const_iterator > MapUnitsByMuacresRange;
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
class TypeHandler< class leaf::open::soi::MapUnit >
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
        leaf::open::soi::MapUnit const& obj,
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
        leaf::open::soi::MapUnit& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::MapUnit& obj,
        leaf::open::soi::MapUnit const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //areaname_legend
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_areaname_legend, Keywords::null, pE );
        //areasymbol_legend
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_areasymbol_legend, defVal.m_areasymbol_legend, pE );
        //brockdepmin
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_brockdepmin, Keywords::null, pE );
        //lkey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_lkey, defVal.m_lkey, pE );
        //muacres
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_muacres, Keywords::null, pE );
        //mukey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_mukey, defVal.m_mukey, pE );
        //muname
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_muname, Keywords::null, pE );
        //musym
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_musym, defVal.m_musym, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
