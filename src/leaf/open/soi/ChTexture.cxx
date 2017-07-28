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
#include <leaf/open/soi/ChTexture.h>

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
ChTexture::ChTexture()
    :
    Object(),
    m_chtkey(),
    m_lieutex()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ChTexture::~ChTexture()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ChTexture::GetChtkey() const
{
    return m_chtkey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& ChTexture::GetLieutex() const
{
    return m_lieutex;
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ChTexturePtr const& chtexture )
{
    os
        << "ChTexture: " << &*chtexture
        << std::endl
        << "  chtkey: "
        << Convert< std::string >( chtexture->GetChtkey() )
        << std::endl
        << "  lieutex: "
        << Convert< std::string >( chtexture->GetLieutex(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
