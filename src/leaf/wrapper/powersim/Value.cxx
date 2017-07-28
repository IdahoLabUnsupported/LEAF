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
#include "Value.h"
#include "Dimension.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
Value::Value(
    ATL::CComPtr< ::IValue > const& iV )
    :
    m_value( iV )
{
    HRESULT hr;
    hr= m_value->get_Dimensions( &m_dimensions );

    hr = m_value->get_DataType( &m_svdt );

    long numDims, numElmnts;
    hr = m_dimensions->get_Count( &numDims );
    m_dimCnt = ( numDims < 0 ) ? 0 : numDims;
    hr = m_dimensions->get_ElementCount( &numElmnts );
    m_elmCnt = ( numElmnts < 0 ) ? 0 : numElmnts;

    VEC outTmp;
    VECS input;
    for( unsigned int i = 0; i < m_dimCnt; ++i )
    {
        unsigned int const& elmntCnt = GetDimension( i )->GetElementCount();
        input.push_back( Vec( elmntCnt ) );
        m_dimSizeVec.push_back( elmntCnt );
    }
    CartesianProduct( m_cartProd, outTmp, input.begin(), input.end() );
    assert( m_elmCnt == m_cartProd.size() );
}
////////////////////////////////////////////////////////////////////////////////
Value::~Value()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var Value::EmptyPDV() const
{
    return PDV( m_dimCnt );
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var Value::GetNumber() const
{
    HRESULT hr;

    ATL::CComVariant v;
    hr = m_value->get_Number( &v );

    VARTYPE varType = v.vt;
    if( m_dimCnt ){ assert( varType & VT_ARRAY ); varType -= VT_ARRAY; }
    switch( m_svdt )
    {
        case simReal:
        {
            assert( varType == VT_R8 );
            if( !m_dimCnt ) return Convert< ATL::CComVariant, double >( v );
            return VariantToCpp< double >( v );
        }
        case simInteger:
        {
            assert( varType == VT_I4 );
            if( !m_dimCnt ) return Convert< ATL::CComVariant, int >( v );
            return VariantToCpp< int >( v );
        }
        case simLogical:
        {
            assert( varType == VT_BOOL );
            if( !m_dimCnt ) return Convert< ATL::CComVariant, bool >( v );
            return VariantToCpp< bool >( v );
        }
        case simComplex:
        case simUndefinedDataType:
        default: return Poco::Dynamic::Var();
    }
}
////////////////////////////////////////////////////////////////////////////////
DimensionPtr Value::GetDimension(
    long const& index ) const
{
    ATL::CComPtr< ::IDimension > iD( NULL );
    HRESULT hr = m_dimensions->get_Item( index, &iD );

    return boost::make_shared< Dimension >( iD );
}
////////////////////////////////////////////////////////////////////////////////
unsigned int const& Value::GetDimensionCount() const
{
    return m_dimCnt;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int const& Value::GetElementCount() const
{
    return m_elmCnt;
}
////////////////////////////////////////////////////////////////////////////////
std::string Value::GetUnitText(
    bool const& plural ) const
{
    HRESULT hr;
    ATL::CComBSTR pResult;
    if( !plural )
    {
        hr = m_value->get_UnitText( simSingularUnitName, &pResult );
    }
    else
    {
        hr = m_value->get_UnitText( simPluralUnitName, &pResult );
    }

    return "";
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Var Value::PDV(
    unsigned int const& size ) const
{
    if( !size ) return Poco::Dynamic::Var();
    return std::vector< Poco::Dynamic::Var >(
        m_dimSizeVec.at( m_dimCnt - size ), PDV( size - 1 ) );
}
////////////////////////////////////////////////////////////////////////////////
void Value::PDV_IDX(
    VEC::const_iterator& itr,
    VEC::const_iterator const& end,
    Poco::Dynamic::Var*& pdv ) const
{
    if( itr == end ) return;
    PDV_IDX( itr, end, pdv = &(*pdv)[ *itr++ ] );
}
////////////////////////////////////////////////////////////////////////////////
void Value::PDV_IDX(
    VEC::const_iterator& itr,
    VEC::const_iterator const& end,
    Poco::Dynamic::Var const*& pdv ) const
{
    if( itr == end ) return;
    PDV_IDX( itr, end, pdv = &(*pdv)[ *itr++ ] );
}
////////////////////////////////////////////////////////////////////////////////
Value::PutNumberResult Value::PutNumber(
    Poco::Dynamic::Var const& pdv )
{
    HRESULT hr;

    VARIANT_BOOL isReadOnly;
    hr = m_value->get_IsReadOnly( &isReadOnly );
    if( Convert< VARIANT_BOOL, bool >( isReadOnly ) )
    {
        return READ_ONLY;
    }

    VARIANT_BOOL canSetNumberNow;
    hr = m_value->get_CanSetNumberNow( &canSetNumberNow );
    if( !Convert< VARIANT_BOOL, bool >( canSetNumberNow ) )
    {
        return CANT_SET_NOW;
    }

    ATL::CComVariant v;
    switch( m_svdt )
    {
        case simReal:
        {
            if( !m_dimCnt ) v = util::Convert< double >( pdv );
            else v = CppToVariant< double >( pdv );
            assert( v.vt & VT_R8 );
            break;
        }
        case simInteger:
        {

            if( !m_dimCnt ) v = util::Convert< int >( pdv );
            else v = CppToVariant< int >( pdv );
            assert( v.vt & VT_I4 );
            break;
        }
        case simLogical:
        {
            if( !m_dimCnt ) v = util::Convert< bool >( pdv );
            else v = CppToVariant< bool >( pdv );
            assert( v.vt & VT_BOOL );
            break;
        }
        case simComplex:
        case simUndefinedDataType:
        default: break;
    }

    hr = m_value->put_Number( v );
    if( FAILED( hr ) )
    {
        return FAILED;
    }

    VARIANT_BOOL isValid;
    hr= m_value->get_Valid( &isValid );
    if( !Convert< VARIANT_BOOL, bool >( isValid ) )
    {
        return NOT_VALID;
    }

    return SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
