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
#include "Helper.h"
#include "ValuePtr.h"
#include "DimensionPtr.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class ModelVariable;

///
class LEAF_POWERSIM_EXPORTS Value
{
public:
    ///Constructor
    Value(
        ATL::CComPtr< ::IValue > const& iV );

    ///Destructor
    ~Value();

    ///Returns unitialized Poco::Dynamic::Var
    ///Has correct dimensions/size for this Value
    Poco::Dynamic::Var EmptyPDV() const;

    ///
    Poco::Dynamic::Var GetNumber() const;

    ///
    DimensionPtr GetDimension(
        long const& index ) const;

    ///
    unsigned int const& GetDimensionCount() const;

    ///
    unsigned int const& GetElementCount() const;

    ///
    std::string GetUnitText(
        bool const& plural = false ) const;

    ///
    enum PutNumberResult
    {
        READ_ONLY = -3,
        CANT_SET_NOW = -2,
        FAILED = -1,
        NOT_VALID = 0,
        SUCCESS = 1
    };
    PutNumberResult PutNumber(
        Poco::Dynamic::Var const& pdv );

protected:

private:
    ///Restrict copying from copy constructor
    Value(
        Value const& );

    ///Restrict copying from equal operator
    Value& operator=(
        Value const& );

    ///
    Poco::Dynamic::Var PDV(
        unsigned int const& size ) const;

    ///
    void PDV_IDX(
        VEC::const_iterator& itr,
        VEC::const_iterator const& end,
        Poco::Dynamic::Var*& pdv ) const;

    ///
    void PDV_IDX(
        VEC::const_iterator& itr,
        VEC::const_iterator const& end,
        Poco::Dynamic::Var const*& pdv ) const;

    ///
    template< typename T >
    Poco::Dynamic::Var VariantToCpp(
        ATL::CComVariant const& v ) const;

    ///
    template< typename T >
    ATL::CComVariant CppToVariant(
        Poco::Dynamic::Var const& pdv ) const;

    ///
    ATL::CComPtr< ::IValue > m_value;

    ///
    ATL::CComPtr< ::IDimensions > m_dimensions;

    ///
    SimVarDataType m_svdt;

    ///
    unsigned int m_dimCnt;

    ///
    unsigned int m_elmCnt;

    ///
    VEC m_dimSizeVec;

    ///
    VECS m_cartProd;

};

///
template< typename T > inline
Poco::Dynamic::Var Value::VariantToCpp(
    ATL::CComVariant const& v ) const
{
    Poco::Dynamic::Var pdv = PDV( m_dimCnt ), *ppdv( NULL );
    ATL::CComSafeArray< T > sa( v.parray );
    VECS::const_iterator itr = m_cartProd.begin();
    for( ; itr != m_cartProd.end(); ++itr )
    {
        VEC const& idxs = *itr;
        T val; HRESULT hr = sa.MultiDimGetAt(
            //This is dangerous!!! Possibly undefined behavior
            reinterpret_cast< LONG const* >( &idxs[ 0 ] ), val );
        PDV_IDX( idxs.begin(), idxs.end(), ppdv = &pdv );
        *ppdv = val;
    }
    return pdv;
}

///There is no bool data type for CComSafeArray
template<> inline
Poco::Dynamic::Var Value::VariantToCpp< bool >(
    ATL::CComVariant const& v ) const
{
    Poco::Dynamic::Var pdv = PDV( m_dimCnt ), *ppdv( NULL );
    ATL::CComSafeArray< VARIANT_BOOL, VT_BOOL > sa( v.parray );
    VECS::const_iterator itr = m_cartProd.begin();
    for( ; itr != m_cartProd.end(); ++itr )
    {
        VEC const& idxs = *itr;
        VARIANT_BOOL val; HRESULT hr = sa.MultiDimGetAt(
            //This is dangerous!!! Possibly undefined behavior
            reinterpret_cast< LONG const* >( &idxs[ 0 ] ), val );
        PDV_IDX( idxs.begin(), idxs.end(), ppdv = &pdv );
        *ppdv = Convert< VARIANT_BOOL, bool >( val );
    }
    return pdv;
}

///
template< typename T > inline
ATL::CComVariant Value::CppToVariant(
    Poco::Dynamic::Var const& pdv ) const
{
    Poco::Dynamic::Var const* ppdv( NULL );
    ATL::CTempBuffer< ATL::CComSafeArrayBound > sabound( m_dimCnt );
    for( unsigned int i = 0; i < m_dimCnt; ++i )
    {
        sabound[ i ].SetLowerBound( 0 );
        sabound[ i ].SetCount( m_dimSizeVec.at( i ) );
    }
    ATL::CComSafeArray< T > sa( sabound, m_dimCnt );
    VECS::const_iterator itr = m_cartProd.begin();
    for( ; itr != m_cartProd.end(); ++itr )
    {
        VEC const& idxs = *itr;
        PDV_IDX( idxs.begin(), idxs.end(), ppdv = &pdv );
        HRESULT hr = sa.MultiDimSetAt(
            //This is dangerous!!! Possibly undefined behavior
            reinterpret_cast< LONG const* >( &idxs[ 0 ] ),
            util::Convert< T >( *ppdv ) );
    }
    return ATL::CComVariant( sa );
}

///There is no bool data type for CComSafeArray
template<> inline
ATL::CComVariant Value::CppToVariant< bool >(
    Poco::Dynamic::Var const& pdv ) const
{
    Poco::Dynamic::Var const* ppdv( NULL );
    ATL::CTempBuffer< ATL::CComSafeArrayBound > sabound( m_dimCnt );
    for( unsigned int i = 0; i < m_dimCnt; ++i )
    {
        sabound[ i ].SetLowerBound( 0 );
        sabound[ i ].SetCount( m_dimSizeVec.at( i ) );
    }
    ATL::CComSafeArray< VARIANT_BOOL, VT_BOOL > sa( sabound, m_dimCnt );
    VECS::const_iterator itr = m_cartProd.begin();
    for( ; itr != m_cartProd.end(); ++itr )
    {
        VEC const& idxs = *itr;
        PDV_IDX( idxs.begin(), idxs.end(), ppdv = &pdv );
        HRESULT hr = sa.MultiDimSetAt(
            //This is dangerous!!! Possibly undefined behavior
            reinterpret_cast< LONG const* >( &idxs[ 0 ] ),
            util::Convert< bool >( *ppdv ) );
    }
    return ATL::CComVariant( sa );
}

} //end powersim
} //end wrapper
} //end leaf
