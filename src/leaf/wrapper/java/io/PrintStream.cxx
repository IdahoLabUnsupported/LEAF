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
#include <leaf/wrapper/java/io/PrintStream.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace io
{

////////////////////////////////////////////////////////////////////////////////
PrintStream::PrintStream(
    jobject const& o )
    :
    lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
PrintStream::PrintStream(
    lang::String const& fileName )
    :
    lang::Object(
        java::NewObject( GetJclass(), GetJmid( "<init>" ), fileName.Jobj() ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
PrintStream::~PrintStream()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
jclass const& PrintStream::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/io/PrintStream" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& PrintStream::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
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
