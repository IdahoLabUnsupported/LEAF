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
#include <leaf/open/Common.h>

#include <leaf/util/Template.h>

// --- Boost Includes --- //
#include <boost/type_traits/is_arithmetic.hpp>

namespace leaf
{
namespace open
{
namespace soi
{

///
namespace SsurgoValType
{

enum Enum
{
    R = 0,
    L = 1,
    H = 2
};

} //end SsurgoValType

///
template< typename T > struct is_ssurgo_vec_type : boost::false_type{};
template<> struct is_ssurgo_vec_type< float > : boost::true_type{};
template<> struct is_ssurgo_vec_type< double > : boost::true_type{};
template<> struct is_ssurgo_vec_type< long double > : boost::true_type{};
template<> struct is_ssurgo_vec_type< Poco::Dynamic::Var > : boost::true_type{};

///
template< typename T, typename SFINAE = void >
struct SSURGO_VEC{};

///
template< typename T >
class SSURGO_VEC< T,
    typename boost::enable_if< is_ssurgo_vec_type< T > >::type >
{
public:
    ///
    typedef typename boost::is_same< T, Poco::Dynamic::Var > TorVar;

    ///
    SSURGO_VEC()
        :
        m_vars()
    {
        //This is for when type must be instantiated with ODBC DE_BOUND
        //m_vars[ 0 ] = T();
        //m_vars[ 1 ] = T();
        //m_vars[ 2 ] = T();
    }

    ///
    Poco::Nullable< T >& operator [](
        unsigned int const& valType )
    {
        poco_assert( valType < 3 );
        return m_vars[ valType ];
    }

    ///
    Poco::Nullable< T > const& operator [](
        unsigned int const& valType ) const
    {
        poco_assert( valType < 3 );
        return m_vars[ valType ];
    }

    ///
    Poco::Nullable< T >& R()
    {
        return m_vars[ SsurgoValType::R ];
    }

    ///
    Poco::Nullable< T > const& R() const
    {
        return m_vars[ SsurgoValType::R ];
    }

    ///
    Poco::Nullable< T >& L()
    {
        return m_vars[ SsurgoValType::L ];
    }

    ///
    Poco::Nullable< T > const& L() const
    {
        return m_vars[ SsurgoValType::L ];
    }

    ///
    Poco::Nullable< T >& H()
    {
        return m_vars[ SsurgoValType::H ];
    }

    ///
    Poco::Nullable< T > const& H() const
    {
        return m_vars[ SsurgoValType::H ];
    }

    void FixUpR()
    {
        FixUpR( TorVar() );
    }

private:
    ///
    void FixUpR(
        boost::true_type )
    {
        if( !( R().isNull() ) || ( L().isNull() || H().isNull() ) )
        {
            return;
        }

        try
        {
            R() = ( L().value().convert< double >() +
                    H().value().convert< double >() ) / 2.0;
        }
        catch( ... )
        {
            return;
        }
    }

    ///
    void FixUpR(
        boost::false_type )
    {
        if( !( R().isNull() ) || ( L().isNull() || H().isNull() ) )
        {
            return;
        }

        try
        {
            R() = ( L().value() + H().value() ) / 2.0;
        }
        catch( ... )
        {
            return;
        }
    }

    ///
    boost::array< Poco::Nullable< T >, 3 > m_vars;

};

} //end soi
} //end open
} //end leaf
