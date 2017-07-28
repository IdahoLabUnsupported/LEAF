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
#include <leaf/wrapper/java/lang/Double.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
double Double::NaN()
{
    static double const NaN =
        java::GetStaticField< jdouble >( GetJclass(), GetJfid( "NaN" ) );
    return NaN;
}
////////////////////////////////////////////////////////////////////////////////
bool Double::IsNaN(
    double const& v )
{
    return( java::CallStaticMethod< jboolean >(
        GetJclass(), GetJmid( "isNaN" ), v ) != JNI_FALSE );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Double::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/Double" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Double::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "isNaN",
          java::GetStaticMethodID(
              GetJclass(), "isNaN",
              "(D)Z" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& Double::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        ( "NaN",
          java::GetStaticFieldID(
              GetJclass(), "NaN",
              "D" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java
} //end wrapper
} //end leaf
