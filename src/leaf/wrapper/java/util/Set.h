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


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/java/lang/Object.h>

#include <leaf/wrapper/java/util/Iterator.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace util
{

template< typename E >
class Set : public lang::Object
{
public:
    ///
    typedef typename boost::shared_ptr< Set< E > > SPtr;

    ///
    Set(
        jobject const& o )
        :
        lang::Object( o )
    {
        ;
    }

    ///
    virtual ~Set()
    {
        ;
    }

    ///
    typename Iterator< E >::SPtr GetIterator() const
    {
        return Create< Iterator< E > >(
            java::CallMethod< jobject >( m_jobject, GetJmid( "iterator" ) ) );
    }

    ///
    static jclass const& GetJclass()
    {
        static jclass const clazz = java::FindClass( "java/util/Set" );
        return clazz;
    }

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name )
    {
        static java::JMIDMAP const jmidMap = boost::assign::map_list_of
            ( "iterator",
              java::GetMethodID(
                  GetJclass(), "iterator",
                  "()Ljava/util/Iterator;" ) );

        java::JMIDMAP::const_iterator itr = jmidMap.find( name );
        return itr->second;
    }

};

} //end util
} //end java
} //end wrapper
} //end leaf
