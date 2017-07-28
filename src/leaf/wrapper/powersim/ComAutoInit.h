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

// --- Windows Includes --- //
#include <ObjBase.h>

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class LEAF_POWERSIM_EXPORTS CComAutoInit
{
public:
    ///
    static CComAutoInit& Instance(
        bool const& erase = false );

protected:

private:
    ///Initializes COM using CoInitialize
    ///On failure, signals error using AtlThrow
    CComAutoInit();

    ///Initializes COM using CoInitializeEx
    ///On failure, signals error using AtlThrow
    explicit CComAutoInit(
        __in DWORD dwCoInit );

    ///Uninitializes COM using CoUninitialize
    ~CComAutoInit();

    ///Restrict copying from copy constructor
    CComAutoInit(
        CComAutoInit const& );

    ///Restrict copying from equal operator
    CComAutoInit& operator=(
        CComAutoInit const& );

};

} //end powersim
} //end wrapper
} //end leaf
