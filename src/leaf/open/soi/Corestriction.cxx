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
#include <leaf/open/soi/Corestriction.h>

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
Corestriction::Corestriction()
    :
    Object(),
    m_corestrictkey(),
    m_resdept()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Corestriction::~Corestriction()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Corestriction::GetCorestrictkey() const
{
    return m_corestrictkey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Corestriction::GetResdept_r() const
{
    return m_resdept[ SsurgoValType::R ];
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& Corestriction::GetResdept(
    unsigned int const& valType ) const
{
    return m_resdept[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    CorestrictionPtr const& corestriction )
{
    os
        << "Corestriction: " << &*corestriction
        << std::endl
        << "  corestrictkey: "
        << Convert< std::string >( corestriction->GetCorestrictkey() )
        << std::endl
        << "  resdept: "
        << Convert< std::string >( corestriction->GetResdept(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
