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
#include <leaf/open/soi/ChFrag.h>

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
ChFrag::ChFrag()
    :
    Object(),
    m_chfragskey(),
    m_fragvol()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ChFrag::~ChFrag()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Var const& ChFrag::GetChfragskey() const
{
    return m_chfragskey;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > const& ChFrag::GetFragvol(
    unsigned int const& valType ) const
{
    return m_fragvol[ valType ];
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ChFragPtr const& chfrag )
{
    os
        << "ChFrag: " << &*chfrag
        << std::endl
        << "  chfragskey: "
        << Convert< std::string >( chfrag->GetChfragskey() )
        << std::endl
        << "  fragvol: "
        << Convert< std::string >( chfrag->GetFragvol(), "null" )
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
