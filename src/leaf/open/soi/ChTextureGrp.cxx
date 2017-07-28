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


// --- LEAF Includes --- //
#include <leaf/open/soi/ChTextureGrp.h>

using namespace leaf::util;

// --- Poco Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace soi
{

////////////////////////////////////////////////////////////////////////////////
ChTextureGrp::ChTextureGrp()
    :
    Object(),
    m_of1( BOOST_ONCE_INIT_CONST ),
    m_chtgkey(),
    m_rvindicator(),
    m_stratextsflag(),
    m_texdesc(),
    m_texture(),
    m_hasTextures( false ),
    m_textures()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ChTextureGrp::~ChTextureGrp()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ChTexturePtr ChTextureGrp::GetTexture(
    std::string const& chtkey,
    bool const& mustExist ) const
{
    ChTexturesByChtkey::const_iterator itr =
        m_textures.get< CHTKEY >().find( chtkey );
    if( itr == m_textures.get< CHTKEY >().end() )
    {
        if( !mustExist ) return ChTexturePtr();

        std::stringstream err;
        err << "leaf::open::soi::ChTextureGrp::GetTexture( \"" << chtkey
            << "\" ) could not find specified chtkey in ChTexture map!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
ChTextures const& ChTextureGrp::GetTextures() const
{
    return m_textures;
}
////////////////////////////////////////////////////////////////////////////////
std::size_t ChTextureGrp::GetNumTextures() const
{
    return m_textures.size();
}
////////////////////////////////////////////////////////////////////////////////
Var const& ChTextureGrp::GetChtgkey() const
{
    return m_chtgkey;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ChTextureGrp::GetRvindicator() const
{
    return m_rvindicator;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ChTextureGrp::GetStratextsflag() const
{
    return m_stratextsflag;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& ChTextureGrp::GetTexdesc() const
{
    return m_texdesc;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& ChTextureGrp::GetTexture() const
{
    return m_texture;
}
////////////////////////////////////////////////////////////////////////////////
bool const& ChTextureGrp::HasTextures() const
{
    return m_hasTextures;
}
////////////////////////////////////////////////////////////////////////////////
void ChTextureGrp::Populate() const
{
    boost::call_once( m_of1, boost::bind( &ChTextureGrp::PopulateTextures, this ) );
}
////////////////////////////////////////////////////////////////////////////////
void ChTextureGrp::PopulateTextures() const
{
    ( GetSession( DB_SSURGO )
        << "select\n"
        << "  t1.chtkey,\n"
        << "  t1.lieutex\n"
        << "from chtexture as t1\n"
        << "where\n"
        << "  t1.chtgkey = ?",
        into( m_textures ),
        useRef( m_chtgkey ),
        now );

    m_hasTextures = ( m_textures.size() != 0 );
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ChTextureGrpPtr const& chtexturegrp )
{
    os
        << "ChTextureGrp: " << &*chtexturegrp
        << std::endl
        << "  chtgkey: "
        << Convert< std::string >( chtexturegrp->GetChtgkey() )
        << std::endl
        << "  rvindicator: "
        << Convert< std::string >( chtexturegrp->GetRvindicator() )
        << std::endl
        << "  stratextsflag: "
        << Convert< std::string >( chtexturegrp->GetStratextsflag() )
        << std::endl
        << "  texdesc: "
        << Convert< std::string >( chtexturegrp->GetTexdesc(), "null" )
        << std::endl
        << "  texture: "
        << Convert< std::string >( chtexturegrp->GetTexture(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
