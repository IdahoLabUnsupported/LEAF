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
#include <leaf/open/Object.h>

// --- Poco Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;

namespace leaf
{
namespace open
{

////////////////////////////////////////////////////////////////////////////////
Object::Object()
    :
    m_struct( Struct< std::string >() )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Object::Object(
    Struct< std::string >::Data const& val )
    :
    m_struct( Struct< std::string >( val ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Object::~Object()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void Object::FromJson(
    std::string const& json )
{
    m_struct = Var::parse( json );
}
////////////////////////////////////////////////////////////////////////////////
std::string Object::ToJson()
{
    return m_struct.convert< std::string >();
}
////////////////////////////////////////////////////////////////////////////////

} //end open
} //end leaf
