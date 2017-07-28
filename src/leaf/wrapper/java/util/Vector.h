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

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace util
{

template< typename E,
    bool isObject = boost::is_base_of< lang::Object, E >::value >
class Vector : public lang::Object
{
public:
    ///
    typedef typename boost::shared_ptr< E > TypeSPtr;
    typedef typename boost::shared_ptr< Vector< E > > SPtr;
    typedef typename leaf::util::Select<
        isObject, TypeSPtr, E >::Result ElementType;

    ///
    Vector()
        :
        lang::Object( java::NewObject( GetJclass(), GetJmid( "<init>" ) ) )
    {
        ;
    }

    ///
    Vector(
        jobject const& o )
        :
        lang::Object( o )
    {
        ;
    }

    ///
    virtual ~Vector()
    {
        ;
    }

    ///
    unsigned int Size() const
    {
        return java::CallMethod< jint >( m_jobject, GetJmid( "size" ) );
    }

    ///
    ElementType ElementAt(
        unsigned int const& index ) const
    {
        return ElementAt( index, leaf::util::Int2Type< isObject >() );
    }

    ///
    static jclass const& GetJclass()
    {
        static jclass const clazz = java::FindClass( "java/util/Vector" );
        return clazz;
    }

protected:

private:
    ///
    ElementType ElementAt(
        unsigned int const& index,
        leaf::util::Int2Type< true > ) const
    {
        return Create< E >( java::CallMethod< jobject >(
            m_jobject, GetJmid( "elementAt" ), index ) );
    }

    ///
    ElementType ElementAt(
        unsigned int const& index,
        leaf::util::Int2Type< false > ) const
    {
        return java::CallMethod< E >(
            m_jobject, GetJmid( "elementAt" ), index );
    }

    ///
    static jmethodID const& GetJmid(
        std::string const& name )
    {
        static java::JMIDMAP const jmidMap = boost::assign::map_list_of
            ( "<init>",
              java::GetMethodID(
                  GetJclass(), "<init>",
                  "()V" ) )
            ( "size",
              java::GetMethodID(
                  GetJclass(), "size",
                  "()I" ) )
            ( "elementAt",
              java::GetMethodID(
                  GetJclass(), "elementAt",
                  "(I)Ljava/lang/Object;" ) );

        java::JMIDMAP::const_iterator itr = jmidMap.find( name );
        return itr->second;
    }

};

} //end util
} //end java
} //end wrapper
} //end leaf
