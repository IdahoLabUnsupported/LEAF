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
#include <leaf/open/man/Management.h>

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
Management::Management()
    :
    m_name(),
    m_operations()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Management::~Management()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ManagementPtr Management::Create()
{
    return boost::make_shared< Management >();
}
////////////////////////////////////////////////////////////////////////////////
void Management::AddOperation(
    Operation const& operation )
{
    m_operations.push_back( operation );
}
////////////////////////////////////////////////////////////////////////////////
Operations const& Management::GetOperations() const
{
    return m_operations;
}
////////////////////////////////////////////////////////////////////////////////
std::size_t Management::GetNumOperations() const
{
    return m_operations.size();
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Management::GetName() const
{
    return m_name;
}
////////////////////////////////////////////////////////////////////////////////
void Management::SetName(
    std::string const& name )
{
    m_name = name;
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    ManagementPtr const& management )
{
    os
        << "Management: " << &*management
        << std::endl
        << "  name: "
        << management->GetName()
        << std::endl;

    return os;
}
////////////////////////////////////////////////////////////////////////////////

} //end soi
} //end open
} //end leaf
