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
#include <leaf/wrapper/java/api-java.h>

#include <leaf/util/Template.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

class Object;
typedef boost::shared_ptr< Object > ObjectPtr;

class LEAF_JAVA_EXPORTS Object : public boost::noncopyable
{
public:
    ///
    virtual ~Object();

    ///
    template< typename T >
    static typename boost::enable_if<
        boost::is_base_of< Object, T >, boost::shared_ptr< T > >::type
    Create()
    {
        return boost::make_shared< T >();
    }

    ///
    template< typename T >
    static typename boost::enable_if<
        boost::is_base_of< Object, T >, boost::shared_ptr< T > >::type
    Create(
        jobject const& o )
    {
        return boost::make_shared< T >( o );
    }

    ///
    static jclass const& GetJclass();

    ///
    virtual jobject Jobj() const;

protected:
    ///
    Object(
        jobject const& o );

    ///
    jobject m_jobject;

private:

};

} //end lang
} //end java
} //end wrapper
} //end leaf
