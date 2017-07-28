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
#include <leaf/wrapper/java/sql/ResultSet.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace sql
{

////////////////////////////////////////////////////////////////////////////////
ResultSet::ResultSet(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ResultSet::~ResultSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
template<>
lang::StringPtr ResultSet::Get< lang::String >(
    lang::String const& columnLabel ) const
{
    return Create< lang::String >( java::CallMethod< jobject >(
        m_jobject, GetJmid( "getString" ), columnLabel.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
template<>
bool ResultSet::Get< bool >(
    lang::String const& columnLabel ) const
{
    return ( java::CallMethod< jboolean >(
        m_jobject, GetJmid( "getBoolean" ), columnLabel.Jobj() ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
template<>
double ResultSet::Get< double >(
    lang::String const& columnLabel ) const
{
    return java::CallMethod< jdouble >(
        m_jobject, GetJmid( "getDouble" ), columnLabel.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
template<>
int ResultSet::Get< int >(
    lang::String const& columnLabel ) const
{
    return java::CallMethod< jint >(
        m_jobject, GetJmid( "getInt" ), columnLabel.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
bool ResultSet::Next() const
{
    return ( java::CallMethod< jboolean >(
        m_jobject, GetJmid( "next" ) ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& ResultSet::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/sql/ResultSet" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& ResultSet::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "next",
          java::GetMethodID(
              GetJclass(), "next",
              "()Z" ) )
        ( "getBoolean",
          java::GetMethodID(
              GetJclass(), "getBoolean",
              "(Ljava/lang/String;)Z" ) )
        ( "getDouble",
          java::GetMethodID(
              GetJclass(), "getDouble",
              "(Ljava/lang/String;)D" ) )
        ( "getInt",
          java::GetMethodID(
              GetJclass(), "getInt",
              "(Ljava/lang/String;)I" ) )
        ( "getString",
          java::GetMethodID(
              GetJclass(), "getString",
              "(Ljava/lang/String;)Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end sql
} //end java
} //end wrapper
} //end leaf
