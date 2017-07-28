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
#include <leaf/wrapper/java/lang/Thread.h>

#include <leaf/wrapper/java/util/Map.h>

// --- Boost Includes --- //
#include <boost/algorithm/string/predicate.hpp>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
Thread::Thread(
    jobject const& o )
    :
    Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ThreadPtr Thread::CurrentThread()
{
    return Create< Thread >(
        java::CallStaticMethod< jobject >(
            GetJclass(), GetJmid( "currentThread" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
util::Map< Thread, util::Array< StackTraceElement > >::SPtr
Thread::GetAllStackTraces()
{
    return Create< util::Map< Thread, util::Array< StackTraceElement > > >(
        java::CallStaticMethod< jobject >(
            GetJclass(), GetJmid( "getAllStackTraces" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
ClassLoaderPtr Thread::GetContextClassLoader() const
{
    return Create< ClassLoader >(
        java::CallMethod< jobject >(
            m_jobject, GetJmid( "getContextClassLoader" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
long Thread::GetId() const
{
    //This conversion may be incorrect
    return static_cast< long >(
        java::CallMethod< jlong >( m_jobject, GetJmid( "getId" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
StringPtr Thread::GetName() const
{
    return Create< String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getName" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
EnumPtr Thread::GetState() const
{
    return Create< Enum >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "getState" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
void Thread::Join() const
{
    java::CallMethod< void >( m_jobject, GetJmid( "join" ) );
}
////////////////////////////////////////////////////////////////////////////////
StringPtr Thread::ToString() const
{
    return Create< String >(
        java::CallMethod< jobject >( m_jobject, GetJmid( "toString" ) ) );
}
////////////////////////////////////////////////////////////////////////////////
void Thread::SetContextClassLoader(
    ClassLoader const& cl )
{
    java::CallMethod< void >(
        m_jobject, GetJmid( "setContextClassLoader" ), cl.Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void Thread::WaitForEDT()
{
    util::Set< Thread >::SPtr threadSet = GetAllStackTraces()->KeySet();

    typedef util::Iterator< Thread > ItrType;
    ItrType::SPtr itr = threadSet->GetIterator();
    while( itr->HasNext() )
    {
        ItrType::TypeSPtr thread = itr->Next();
        std::string name = thread->GetName()->StdString();
        if( boost::starts_with( name, "AWT-EventQueue" ) ) thread->Join();
    }
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Thread::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/Thread" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& Thread::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "currentThread",
          java::GetStaticMethodID(
              GetJclass(), "currentThread",
              "()Ljava/lang/Thread;" ) )
        ( "getAllStackTraces",
          java::GetStaticMethodID(
              GetJclass(), "getAllStackTraces",
              "()Ljava/util/Map;" ) )
        ( "getContextClassLoader",
          java::GetMethodID(
              GetJclass(), "getContextClassLoader",
              "()Ljava/lang/ClassLoader;" ) )
        ( "getId",
          java::GetMethodID(
              GetJclass(), "getId",
              "()J" ) )
        ( "getName",
          java::GetMethodID(
              GetJclass(), "getName",
              "()Ljava/lang/String;" ) )
        ( "getState",
          java::GetMethodID(
              GetJclass(), "getState",
              "()Ljava/lang/Thread$State;" ) )
        ( "join",
          java::GetMethodID(
              GetJclass(), "join",
              "()V" ) )
        ( "setContextClassLoader",
          java::GetMethodID(
              GetJclass(), "setContextClassLoader",
              "(Ljava/lang/ClassLoader;)V" ) )
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
