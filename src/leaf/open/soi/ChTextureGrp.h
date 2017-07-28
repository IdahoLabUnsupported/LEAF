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
#include <leaf/open/soi/ChTextureGrpPtr.h>
#include <leaf/open/soi/ChTexture.h>

#ifndef SWIG
struct CHTGKEY{};
#endif //SWIG

namespace leaf
{
namespace open
{
namespace soi
{

class LEAF_OPEN_EXPORTS ChTextureGrp : public Object
{
public:
    ///
    virtual ~ChTextureGrp();

    ///
    ChTexturePtr GetTexture(
        std::string const& chtkey,
        bool const& mustExist = false ) const;

#ifndef SWIG
    ///
    ChTextures const& GetTextures() const;
#endif //SWIG

    ///
    std::size_t GetNumTextures() const;

    ///
    Poco::Dynamic::Var const& GetChtgkey() const;

    ///
    Poco::Dynamic::Var const& GetRvindicator() const;

    ///
    Poco::Dynamic::Var const& GetStratextsflag() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetTexdesc() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetTexture() const;

    ///
    bool const& HasTextures() const;

    ///
    void Populate() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ChTextureGrpPtr const& chtexturegrp );
#endif //SWIG

protected:
    ///
    ChTextureGrp();

private:
    ///
    friend class CHorizon;
    friend class Poco::Data::TypeHandler< ChTextureGrp >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< ChTextureGrp >(
        void*, ChTextureGrp const& );
    friend ChTextureGrpPtr boost::make_shared< ChTextureGrp >();

    ///
    mutable boost::once_flag m_of1;
    void PopulateTextures() const;

    ///varchar( 30 ) not null
    Poco::Dynamic::Var m_chtgkey;

    ///char( 3 ) not null
    Poco::Dynamic::Var m_rvindicator;

    ///char( 3 ) not null
    Poco::Dynamic::Var m_stratextsflag;

    ///text
    Poco::Nullable< Poco::Dynamic::Var > m_texdesc;

    ///varchar( 30 )
    Poco::Nullable< Poco::Dynamic::Var > m_texture;

    ///
    mutable bool m_hasTextures;

    ///
    mutable ChTextures m_textures;

};

#ifndef SWIG
///Define a multiply indexed set with indices by chtgkey
typedef boost::multi_index_container<
    ChTextureGrpPtr,
    boost::multi_index::indexed_by<
        //Sort by chtgkey
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< CHTGKEY >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN( ChTextureGrp,
                Poco::Dynamic::Var const&, GetChtgkey ),
            util::less_than< std::string >
        >
    >
> ChTextureGrps;

///
typedef ChTextureGrps::index< CHTGKEY >::type ChTextureGrpsByChtgkey;
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
class TypeHandler< class leaf::open::soi::ChTextureGrp >
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
        leaf::open::soi::ChTextureGrp const& obj,
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
        leaf::open::soi::ChTextureGrp& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::soi::ChTextureGrp& obj,
        leaf::open::soi::ChTextureGrp const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //chtgkey
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_chtgkey, defVal.m_chtgkey, pE );
        //rvindicator
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_rvindicator, defVal.m_rvindicator, pE );
        //stratextsflag
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_stratextsflag, defVal.m_stratextsflag, pE );
        //texdesc
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_texdesc, Keywords::null, pE );
        //texture
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_texture, Keywords::null, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
