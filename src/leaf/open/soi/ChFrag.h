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
#include <leaf/open/soi/ChFragPtr.h>

#ifndef SWIG
struct CHFRAGSKEY{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS ChFrag : public Object
{
public:
    ///
    virtual ~ChFrag();

    ///
    Poco::Dynamic::Var const& GetChfragskey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetFragvol(
        unsigned int const& valType = SsurgoValType::R ) const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ChFragPtr const& chfrag );
#endif //SWIG

protected:
    ///
    ChFrag();

private:
    ///
    friend class CHorizon;
    friend class Poco::Data::TypeHandler< ChFrag >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< ChFrag >(
        void*, ChFrag const& );
    friend ChFragPtr boost::make_shared< ChFrag >();

    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_chfragskey;

    ///smallint
    SSURGO_VEC< Poco::Dynamic::Var > m_fragvol;

};

#ifndef SWIG
///Define a multiply indexed set with indices by chtgkey
typedef boost::multi_index_container<
    ChFragPtr,
    boost::multi_index::indexed_by<
        //Sort by chfragskey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CHFRAGSKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( ChFrag,
                Poco::Dynamic::Var const&, GetChfragskey ),
            util::less_than< std::string >
        >
    >
> ChFrags;

typedef ChFrags::index< CHFRAGSKEY >::type ChFragsByChfragskey;
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
class TypeHandler< class leaf::open::soi::ChFrag >
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
        leaf::open::soi::ChFrag const& obj,
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
        leaf::open::soi::ChFrag& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::ChFrag& obj,
        leaf::open::soi::ChFrag const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //chfragskey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_chfragskey, defVal.m_chfragskey, pE );
        //fragvol
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_fragvol.R(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_fragvol.L(), Keywords::null, pE );
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_fragvol.H(), Keywords::null, pE );
        obj.m_fragvol.FixUpR();
    }

};
#endif //SWIG

} //end Data
} //end Poco
