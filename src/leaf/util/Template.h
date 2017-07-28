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

// --- Boost Includes --- //
#include <boost/type_traits.hpp>

#include <boost/utility/enable_if.hpp>

namespace leaf
{
namespace util
{

///
template< int v >
struct Int2Type
{
    enum{ value = v };
};

///
template< typename T >
struct Type2Type
{
    typedef T Type;
};

///
template< bool flag, typename T, typename U >
struct Select
{
    typedef T Result;
};

///
template< typename T, typename U >
struct Select< false, T, U >
{
    typedef U Result;
};

} //end util
} //end leaf
