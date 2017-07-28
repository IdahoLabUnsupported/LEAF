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
#include <leaf/wrapper/java/lang/Throwable.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
Throwable::Throwable(
    jobject const& o )
    :
    Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Throwable::~Throwable()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
StringPtr Throwable::GetMessage() const
{
    return Create< String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getMessage" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
util::Array< StackTraceElement >::SPtr Throwable::GetStackTrace() const
{
    return Create< util::Array< StackTraceElement > >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getStackTrace" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
void Throwable::ThrowRuntimeException() const
{
    std::ostringstream err;
    std::string description = ToString()->StdString();
    err << description;

    util::Array< lang::StackTraceElement >::SPtr stackTrace = GetStackTrace();
    for( int i = 0; i < stackTrace->GetLength(); ++i )
    {
        lang::StackTraceElementPtr stackTraceElement =
            stackTrace->GetElement( i );
        std::string trace = stackTraceElement->ToString()->StdString();
        err << std::endl << "    " << trace;
    }

    throw std::runtime_error( err.str().c_str() );
}
////////////////////////////////////////////////////////////////////////////////
StringPtr Throwable::ToString() const
{
    return Create< String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "toString" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Throwable::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/Throwable" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Throwable::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "getMessage",
          java::GetMethodID(
              GetJclass(), "getMessage",
              "()Ljava/lang/String;" ) )
        ( "getStackTrace",
          java::GetMethodID(
              GetJclass(), "getStackTrace",
              "()[Ljava/lang/StackTraceElement;" ) )
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
