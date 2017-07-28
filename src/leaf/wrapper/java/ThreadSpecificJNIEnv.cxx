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
#include <leaf/wrapper/java/ThreadSpecificJNIEnv.h>
#include <leaf/wrapper/java/api-java.h>

// --- Boost Includes --- //
#include <boost/thread.hpp>


namespace leaf
{
namespace wrapper
{
namespace java
{

////////////////////////////////////////////////////////////////////////////////
ThreadSpecificJNIEnv::ThreadSpecificJNIEnv()
    :
    m_attached( false ),
    m_jniEnv( NULL )
{
    std::cout << "Attaching thread to JavaVM: "
        << boost::this_thread::get_id() << std::endl;
    jint ret = VM::Instance().AttachCurrentThread( (void**)&m_jniEnv, NULL );
    m_attached = CheckJNIFunctionReturn( ret );
    assert( m_attached );
}
////////////////////////////////////////////////////////////////////////////////
ThreadSpecificJNIEnv::ThreadSpecificJNIEnv(
    JNIEnv* jniEnv )
    :
    m_attached( false ),
    m_jniEnv( jniEnv )
{
    assert( m_jniEnv != NULL );
}
////////////////////////////////////////////////////////////////////////////////
ThreadSpecificJNIEnv::~ThreadSpecificJNIEnv()
{
    if( !m_attached )
    {
        return;
    }

    std::cout << "Detaching thread from JavaVM: "
        << boost::this_thread::get_id() << std::endl;
    jint ret = VM::Instance().DetachCurrentThread();
    assert( CheckJNIFunctionReturn( ret ) );
}
////////////////////////////////////////////////////////////////////////////////
JNIEnv* ThreadSpecificJNIEnv::GetJNIEnv() const
{
    return m_jniEnv;
}
////////////////////////////////////////////////////////////////////////////////

} //end java
} //end wrapper
} //end leaf
