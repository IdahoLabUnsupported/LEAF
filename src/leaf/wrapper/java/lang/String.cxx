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
#include <leaf/wrapper/java/lang/String.h>

using namespace leaf::util;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
String::String()
    :
    Object( java::NewObject( GetJclass(), GetJmid( "<init>" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
String::String(
    jobject const& o )
    :
    Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
String::String(
    char const* const& cstr )
    :
    Object( NewString( cstr ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
String::String(
    std::string const& str )
    :
    Object( NewString( str ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
String::String(
    Var const& var )
    :
    Object( NULL )
{
    std::string val;
    m_jobject = Convert< std::string >( var, val ) ? NewString( val ) : NULL;
}
////////////////////////////////////////////////////////////////////////////////
String::String(
    Nullable< Var > const& var )
    :
    Object( NULL )
{
    std::string val;
    m_jobject = Convert< std::string >( var, val ) ? NewString( val ) : NULL;
}
////////////////////////////////////////////////////////////////////////////////
String::~String()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
jstring String::NewString(
    std::string const& str )
{
    return java::NewString( str );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& String::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/String" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& String::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "()V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jstring String::Jobj() const
{
    return static_cast< jstring >( m_jobject );
}
////////////////////////////////////////////////////////////////////////////////
std::string String::StdString() const
{
    if( !m_jobject )
    {
        throw std::runtime_error( "jobject is NULL!" );
    }

    char const* chars = java::GetStringChars( Jobj(), JNI_FALSE );
    std::string str( chars );
    java::ReleaseStringChars( Jobj(), chars );

    return str;
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java
} //end wrapper
} //end leaf
