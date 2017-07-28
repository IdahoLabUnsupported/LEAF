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
#include <leaf/open/man/Operation.h>

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
Operation::Operation()
    :
    m_date(),
    m_name(),
    m_hasCrop( false ),
    m_crop()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Operation::Operation(
    std::string const& name )
    :
    m_date(),
    m_name( name ),
    m_hasCrop( false ),
    m_crop()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Operation::Operation(
    Operation const& operation )
    :
    m_date( operation.m_date ),
    m_name( operation.m_name ),
    m_hasCrop( operation.m_hasCrop ),
    m_crop( operation.m_crop )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1800
Operation::Operation(
    Operation&& operation )
    :
    Operation()
{
    swap( *this, operation );
}
#endif
////////////////////////////////////////////////////////////////////////////////
Operation::~Operation()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Operation& Operation::operator =(
    Operation operation )
{
    swap( *this, operation );
    return *this;
}
////////////////////////////////////////////////////////////////////////////////
void swap(
    Operation& first,
    Operation& second )
{
    using std::swap;
    swap( first.m_date, second.m_date );
    swap( first.m_name, second.m_name );
    swap( first.m_hasCrop, second.m_hasCrop );
    swap( first.m_crop, second.m_crop );
}
////////////////////////////////////////////////////////////////////////////////
CropPtr Operation::GetCrop() const
{
    return m_crop;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Operation::GetDate() const
{
    return m_date;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Operation::GetName() const
{
    return m_name;
}
////////////////////////////////////////////////////////////////////////////////
bool const& Operation::HasCrop() const
{
    return m_hasCrop;
}
////////////////////////////////////////////////////////////////////////////////
void Operation::Reset()
{
    m_date = "";
    m_name = "";
    SetCrop( CropPtr() );
}
////////////////////////////////////////////////////////////////////////////////
void Operation::SetCrop(
    Crop const& crop )
{
    SetCrop( Crop::Create( crop ) );
}
////////////////////////////////////////////////////////////////////////////////
void Operation::SetCrop(
    CropPtr const& crop )
{
    m_crop = crop;
    m_hasCrop = ( m_crop != CropPtr() );
}
////////////////////////////////////////////////////////////////////////////////
void Operation::SetDate(
    std::string const& date )
{
    m_date = date;
}
////////////////////////////////////////////////////////////////////////////////
void Operation::SetName(
    std::string const& name )
{
    m_name = name;
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    Operation const& operation )
{
    os
        << "Operation: " << &operation
        << std::endl
        << "  name: "
        << operation.GetName()
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end man
} //end open
} //end leaf
