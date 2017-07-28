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
#include <leaf/wrapper/java/awt/Frame.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace awt
{

////////////////////////////////////////////////////////////////////////////////
Frame::Frame(
    jobject const& o )
    :
    Window( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Frame::~Frame()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
lang::StringPtr Frame::GetTitle() const
{
    return lang::Object::Create< lang::String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getTitle" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Frame::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/awt/Frame" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Frame::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getTitle",
          java::GetMethodID(
              GetJclass(), "getTitle",
              "()Ljava/lang/String;" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end awt
} //end java
} //end wrapper
} //end leaf
