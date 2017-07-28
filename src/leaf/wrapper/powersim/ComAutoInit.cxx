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


// --- Windows Includes --- //
#include <WinError.h>

// --- ATL Includes --- //
#include <atlbase.h>
#include <atldef.h>
#include <atltrace.h>

// --- LEAF Includes --- //
#include "ComAutoInit.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
CComAutoInit::CComAutoInit()
{
    HRESULT hr = CoInitialize( NULL );
    if( FAILED( hr ) )
    {
        ATLTRACE2( TEXT(
            "CoInitialize() failed in CComAutoInit constructor \
            (hr=0x%08X). \n" ), hr );
        ATL::AtlThrowImpl( hr );
    }
}
////////////////////////////////////////////////////////////////////////////////
CComAutoInit::CComAutoInit(
    __in DWORD dwCoInit )
{
    HRESULT hr = CoInitializeEx( NULL, dwCoInit );
    if( FAILED( hr ) )
    {
        ATLTRACE2( TEXT(
            "CoInitializeEx() failed in CComAutoInit constructor \
            (hr=0x%08X).\n" ), hr );
        ATL::AtlThrowImpl( hr );
    }
}
////////////////////////////////////////////////////////////////////////////////
CComAutoInit::~CComAutoInit()
{
    CoUninitialize();
}
////////////////////////////////////////////////////////////////////////////////
CComAutoInit& CComAutoInit::Instance(
    bool const& erase )
{
    static CComAutoInit* s_cComAutoInit = new CComAutoInit();
    if( erase )
    {
        delete s_cComAutoInit;
        s_cComAutoInit = NULL;
    }

    return *s_cComAutoInit;
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
