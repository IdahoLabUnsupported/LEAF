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

#include "DimensionPtr.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

class Value;

class LEAF_POWERSIM_EXPORTS Dimension
{
public:
    ///Constructor
    Dimension(
        ATL::CComPtr< ::IDimension > const& iD );

    ///Destructor
    ~Dimension();

    ///
    unsigned int const& GetElementCount() const;

protected:


private:
    ///Restrict copying from copy constructor
    Dimension(
        Dimension const& );

    ///Restrict copying from equal operator
    Dimension& operator=(
        Dimension const& );

    ///
    ATL::CComPtr< ::IDimension > m_dimension;

    ///
    unsigned int m_elemCnt;

};

} //end powersim
} //end wrapper
} //end leaf
