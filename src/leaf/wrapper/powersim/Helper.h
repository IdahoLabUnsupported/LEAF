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
#include "Common.h"
#include <leaf/util/DataHelper.h>

// --- STL Includes --- //
#include <cassert>
#include <string>
#include <set>
#include <vector>
#include <iostream>

namespace leaf
{
namespace wrapper
{
namespace powersim
{

///
template< typename from, typename to > inline
to Convert(
    from const& f );

///
template<> inline
VARIANT_BOOL Convert< bool, VARIANT_BOOL >(
    bool const& f )
{
    return f ? VARIANT_TRUE : VARIANT_FALSE;
}

///
template<> inline
bool Convert< VARIANT_BOOL, bool >(
    VARIANT_BOOL const& f )
{
    return ( f == VARIANT_TRUE ) ? true : false;
}

///
template<> inline
ATL::CComVariant Convert< bool, ATL::CComVariant >(
    bool const& f )
{
    return Convert< bool, VARIANT_BOOL >( f );
}

///
template<> inline
bool Convert< ATL::CComVariant, bool >(
    ATL::CComVariant const& f )
{
    return Convert< VARIANT_BOOL, bool >( f.boolVal );
}

///
template<> inline
int Convert< ATL::CComVariant, int >(
    ATL::CComVariant const& f )
{
    return f.lVal;
}

///
template<> inline
double Convert< ATL::CComVariant, double >(
    ATL::CComVariant const& f )
{
    return f.dblVal;
}

///
typedef std::vector< unsigned int > VEC;
typedef std::vector< VEC > VECS;

///
class Inc
{
public:
    ///
    Inc()
        :
        m_i( -1 )
    {
        ;
    }

    unsigned int operator ()()
    {
        return ++m_i;
    }

private:
    ///
    int m_i;

};

///
inline
VEC Vec(
    unsigned int const& size )
{
    VEC vec( size );
    std::generate( vec.begin(), vec.end(), Inc() );

    return vec;
}

///
inline
std::ostream& operator <<(
    std::ostream& os,
    VEC const& vec )
{
    for( VEC::const_iterator itr = vec.begin(); itr != vec.end(); ++itr )
    {
        os << "[" << *itr << "]";
    }

    return os;
}

///
inline
std::ostream& operator <<(
    std::ostream& os,
    VECS const& vecs )
{
    for( VECS::const_iterator itr = vecs.begin();  itr != vecs.end(); ++itr )
    {
        os << *itr << std::endl;
    }

    return os;
}

///
inline
void CartesianProduct(
    VECS& vecs,
    VEC& vec,
    VECS::const_iterator const& me,
    VECS::const_iterator const& end )
{
    //
    if( me == end )
    {
        vecs.push_back( vec );
        return;
    }

    //
    VEC const& curr = *me;
    for( VEC::const_iterator itr = curr.begin(); itr != curr.end(); ++itr )
    {
        vec.push_back( *itr );
        CartesianProduct( vecs, vec, me + 1, end );
        vec.pop_back();
    }
}

} //end powersim
} //end wrapper
} //end leaf
