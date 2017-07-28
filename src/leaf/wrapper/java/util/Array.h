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
class Array : public lang::Object
{
public:
    ///
    typedef typename boost::shared_ptr< E > TypeSPtr;
    typedef typename boost::shared_ptr< Array< E > > SPtr;
    typedef typename leaf::util::Select<
        isObject, TypeSPtr, E >::Result ElementType;

    ///
    Array(
        jobject const& o )
        :
        lang::Object( o )
    {
        ;
    }

    ///
    virtual ~Array()
    {
        ;
    }

    ///
    ElementType GetElement(
        unsigned int const& index ) const
    {
        return GetElement( index, leaf::util::Int2Type< isObject >() );
    }

    ///
    unsigned int GetLength() const
    {
        return java::GetArrayLength( Jobj() );
    }

    ///
    void SetElement(
        unsigned int const& index,
        E const& value ) const
    {
        SetElement( index, value, leaf::util::Int2Type< isObject >() );
    }

    ///
    virtual jarray Jobj() const
    {
        return static_cast< jarray >( m_jobject );
    }

protected:

private:
    ///
    ElementType GetElement(
        unsigned int const& index,
        leaf::util::Int2Type< true > ) const
    {
        return Create< E >( java::GetArrayElement< jobject >( Jobj(), index ) );
    }

    ///
    ElementType GetElement(
        unsigned int const& index,
        leaf::util::Int2Type< false > ) const
    {
        return java::GetArrayElement< E >( Jobj(), index );
    }

    ///
    void SetElement(
        unsigned int const& index,
        E const& value,
        leaf::util::Int2Type< true > ) const
    {
        java::SetArrayElement< jobject >( Jobj(), index, value.Jobj() );
    }

    ///
    void SetElement(
        unsigned int const& index,
        E const& value,
        leaf::util::Int2Type< false > ) const
    {
        java::SetArrayElement< E >( Jobj(), index, value );
    }

};

} //end util
} //end java
} //end wrapper
} //end leaf
