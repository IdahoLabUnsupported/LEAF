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
#include <leaf/wrapper/java/util/Properties.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
Properties::Properties()
    :
    Object( java::NewObject( GetJclass(), GetJmid( "<init>" ) ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Properties::Properties(
    jobject const& o )
    :
    lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Properties::~Properties()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
lang::StringPtr Properties::GetProperty(
    lang::String const& key ) const
{
    return Create< lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getProperty" ), key.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
void Properties::Load(
    io::InputStream const& inStream ) const
{
    java::CallMethod< void >( m_jobject, GetJmid( "load" ), inStream.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
lang::StringPtr Properties::SetProperty(
    lang::String const& key,
    lang::String const& value ) const
{
    return Create< lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "setProperty" ), key.Jobj(), value.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Properties::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/util/Properties" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Properties::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getProperty",
          java::GetMethodID(
              GetJclass(), "getProperty",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "()V" ) )
        ( "load",
          java::GetMethodID(
              GetJclass(), "load",
              "(Ljava/io/InputStream;)V" ) )
        ( "setProperty",
          java::GetMethodID(
              GetJclass(), "setProperty",
              "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Object;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end java
} //end wrapper
} //end leaf
