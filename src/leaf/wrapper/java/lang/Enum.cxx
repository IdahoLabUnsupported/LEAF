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
#include <leaf/wrapper/java/lang/Enum.h>
#include <leaf/wrapper/java/lang/String.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
Enum::Enum(
    jobject const& o )
    :
    Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Enum::~Enum()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
StringPtr Enum::ToString() const
{
    return Create< String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "toString" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Enum::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/Enum" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Enum::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "toString",
          java::GetMethodID(
              GetJclass(), "toString",
              "()Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java
} //end wrapper
} //end leaf
