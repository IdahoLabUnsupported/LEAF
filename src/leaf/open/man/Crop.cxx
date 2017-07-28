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
#include <leaf/open/man/Crop.h>

using namespace leaf::util;

// --- Poco Includes --- //
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace man
{

////////////////////////////////////////////////////////////////////////////////
Crop::Crop()
    :
    m_name()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CropPtr Crop::Create()
{
    return boost::make_shared< Crop >();
}
////////////////////////////////////////////////////////////////////////////////
Crop::Crop(
    std::string const& name )
    :
    m_name( name )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Crop::Crop(
    Crop const& crop )
    :
    m_name( crop.m_name )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
CropPtr Crop::Create(
    Crop const& crop )
{
    return boost::make_shared< Crop >( crop );
}
////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1800
Crop::Crop(
    Crop&& crop )
    :
    Crop()
{
    swap( *this, crop );
}
#endif
////////////////////////////////////////////////////////////////////////////////
Crop::~Crop()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Crop& Crop::operator =(
    Crop crop )
{
    swap( *this, crop );
    return *this;
}
////////////////////////////////////////////////////////////////////////////////
void swap(
    Crop& first,
    Crop& second )
{
    using std::swap;
    swap( first.m_name, second.m_name );
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Crop::GetName() const
{
    return m_name;
}
////////////////////////////////////////////////////////////////////////////////
void Crop::SetName(
    std::string const& name )
{
    m_name = name;
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    Crop const& crop )
{
    os
        << "Crop: " << &crop
        << std::endl
        << "  name: "
        << crop.GetName()
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end man
} //end open
} //end leaf
