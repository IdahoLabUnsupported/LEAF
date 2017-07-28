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
#include <leaf/wrapper/java/io/File.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace io
{

////////////////////////////////////////////////////////////////////////////////
File::File(
    jobject const& o )
    :
    lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
File::File(
    lang::String const& pathname )
    :
    lang::Object( NULL )
{
    m_jobject = java::NewObject(
        GetJclass(), GetJmid( "<init>" ), pathname.Jobj() );
    if( Exists() ) return;

    java::lang::StringPtr abspath = GetAbsolutePath();
    java::DeleteGlobalRef( m_jobject );
    m_jobject = java::NewObject(
        GetJclass(), GetJmid( "<init>" ), abspath->Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
File::~File()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
bool File::Exists() const
{
    return ( java::CallMethod< jboolean >(
        m_jobject, GetJmid( "exists" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
lang::StringPtr File::GetAbsolutePath() const
{
    return Create< lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getAbsolutePath" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
lang::StringPtr File::GetCanonicalPath() const
{
    return Create< lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getCanonicalPath" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& File::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/io/File" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& File::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "exists",
          java::GetMethodID(
              GetJclass(), "exists",
              "()Z" ) )
        ( "getAbsolutePath",
          java::GetMethodID(
              GetJclass(), "getAbsolutePath",
              "()Ljava/lang/String;" ) )
        ( "getCanonicalPath",
          java::GetMethodID(
              GetJclass(), "getCanonicalPath",
              "()Ljava/lang/String;" ) )
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(Ljava/lang/String;)V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end io
} //end java
} //end wrapper
} //end leaf
