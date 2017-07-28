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
#include <leaf/wrapper/java/lang/System.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
System::~System()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
StringPtr System::GetProperty(
    String const& key )
{
    return Object::Create< String >( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "getProperty" ), key.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
void System::SetOut(
    io::PrintStream const& out )
{
    java::CallStaticMethod< void >(
        GetJclass(), GetJmid( "setOut" ), out.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void System::SetErr(
    io::PrintStream const& err )
{
    java::CallStaticMethod< void >(
        GetJclass(), GetJmid( "setErr" ), err.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
StringPtr System::SetProperty(
    String const& key,
    String const& value )
{
    return Object::Create< String >( java::CallStaticMethod< jobject >(
        GetJclass(), GetJmid( "setProperty" ), key.Jobj(), value.Jobj() ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& System::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/System" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& System::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getProperty",
          java::GetStaticMethodID(
              GetJclass(), "getProperty",
              "(Ljava/lang/String;)Ljava/lang/String;" ) )
        ( "setOut",
          java::GetStaticMethodID(
              GetJclass(), "setOut",
              "(Ljava/io/PrintStream;)V" ) )
        ( "setErr",
          java::GetStaticMethodID(
              GetJclass(), "setErr",
              "(Ljava/io/PrintStream;)V" ) )
        ( "setProperty",
          java::GetStaticMethodID(
              GetJclass(), "setProperty",
              "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java
} //end wrapper
} //end leaf
