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
#include <leaf/wrapper/java/lang/Object.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

////////////////////////////////////////////////////////////////////////////////
Object::Object(
    jobject const& o )
    :
    m_jobject( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Object::~Object()
{
    java::DeleteGlobalRef( m_jobject );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& Object::GetJclass()
{
    static jclass const clazz = java::FindClass( "java/lang/Object" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jobject Object::Jobj() const
{
    return m_jobject;
}
////////////////////////////////////////////////////////////////////////////////

} //end lang
} //end java
} //end wrapper
} //end leaf
