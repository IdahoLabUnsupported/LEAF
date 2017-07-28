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
#include "ModelVariable.h"
#include "Value.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
ModelVariable::ModelVariable(
    ATL::CComPtr< ::IModelVariable > const& iMV )
    :
    m_modelVariable( iMV )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ModelVariable::~ModelVariable()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ValuePtr ModelVariable::GetValue(
    std::string const& subscript ) const
{
    ATL::CComPtr< ::IValue > iV( NULL );
    HRESULT hr = m_modelVariable->get_Value(
        ATL::CComVariant( subscript.c_str() ), simActualData, &iV );

    return boost::make_shared< Value >( iV );
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
