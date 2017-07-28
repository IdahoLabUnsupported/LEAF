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
#include <leaf/open/soi/ChTexturePtr.h>

#ifndef SWIG
struct CHTKEY{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS ChTexture : public Object
{
public:
    ///
    virtual ~ChTexture();

    ///
    Poco::Dynamic::Var const& GetChtkey() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetLieutex() const;

protected:
    ///
    ChTexture();

private:
    ///
    friend class ChTextureGrp;
    friend class Poco::Data::TypeHandler< ChTexture >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< ChTexture >(
        void*, ChTexture const& );
    friend ChTexturePtr boost::make_shared< ChTexture >();

    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_chtkey;

    ///varchar( 254 )
    Poco::Nullable< Poco::Dynamic::Var > m_lieutex;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ChTexturePtr const& chtexture );
#endif //SWIG

};

#ifndef SWIG
///Define a multiply indexed set with indices by chtkey
typedef boost::multi_index_container<
    ChTexturePtr,
    boost::multi_index::indexed_by<
        //Sort by chtkey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CHTKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( ChTexture,
                Poco::Dynamic::Var const&, GetChtkey ),
            util::less_than< std::string >
        >
    >
> ChTextures;

typedef ChTextures::index< CHTKEY >::type ChTexturesByChtkey;
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
class TypeHandler< class leaf::open::soi::ChTexture >
{
public:
    ///
    static std::size_t size()
    {
        //We handle two columns of the Table
        return 2;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::soi::ChTexture const& obj,
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
        leaf::open::soi::ChTexture& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::ChTexture& obj,
        leaf::open::soi::ChTexture const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //chtkey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_chtkey, defVal.m_chtkey, pE );
        //lieutex
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_lieutex, Keywords::null, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
