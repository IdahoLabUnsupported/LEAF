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
#include <leaf/open/soi/CorestrictionPtr.h>

#ifndef SWIG
struct CORESTRICTKEY{};
struct RESDEPT{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS Corestriction : public Object
{
public:
    ///
    virtual ~Corestriction();

    ///
    Poco::Dynamic::Var const& GetCorestrictkey() const;

    ///For boost::multi_index
    Poco::Nullable< Poco::Dynamic::Var > const& GetResdept_r() const;
    ///For everyone else
    Poco::Nullable< Poco::Dynamic::Var > const& GetResdept(
        unsigned int const& valType = SsurgoValType::R ) const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        CorestrictionPtr const& corestriction );
#endif //SWIG

protected:
    ///
    Corestriction();

private:
    ///
    friend class Component;
    friend class Poco::Data::TypeHandler< Corestriction >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Corestriction >(
        void*, Corestriction const& );
    friend CorestrictionPtr boost::make_shared< Corestriction >();

    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_corestrictkey;

    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_resdept;

};

#ifndef SWIG
///Define a multiply indexed set with indices by cokey
typedef boost::multi_index_container<
    CorestrictionPtr,
    boost::multi_index::indexed_by<
        //Sort by corestrictkey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CORESTRICTKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( Corestriction,
                Poco::Dynamic::Var const&, GetCorestrictkey ),
            util::less_than< std::string >
        >,

        //Sort by resdept_r
        boost::multi_index::ordered_non_unique<
            boost::multi_index::tag< RESDEPT >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( Corestriction,
                Poco::Nullable< Poco::Dynamic::Var > const&, GetResdept_r ),
            util::less_than< unsigned int >
        >
    >
> Corestrictions;

///
typedef Corestrictions::index< CORESTRICTKEY >::type CorestrictionsByCorestrictkey;
typedef Corestrictions::index< RESDEPT >::type CorestrictionsByResdept;
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
class TypeHandler< class leaf::open::soi::Corestriction >
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
        leaf::open::soi::Corestriction const& obj,
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
        leaf::open::soi::Corestriction& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::Corestriction& obj,
        leaf::open::soi::Corestriction const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //corestrictkey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_corestrictkey, defVal.m_corestrictkey, pE );
        //resdept
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_resdept.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_resdept.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_resdept.H(), Keywords::null, pE );
        obj.m_resdept.FixUpR();
    }

};
#endif //SWIG

} //end Data
} //end Poco
