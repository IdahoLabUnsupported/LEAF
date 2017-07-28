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
#include <leaf/open/spa/Spatial.h>

// --- Boost Includes --- //
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace open
{
namespace spa
{

////////////////////////////////////////////////////////////////////////////////
Spatial::Spatial()
    :
    Object()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Spatial::~Spatial()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ShapePtr Spatial::GetShape(
    unsigned int const& objId ) const
{
    ShapesByObjId::const_iterator itr =
        m_shapes.get< OBJID >().find( objId );
    if( itr == m_shapes.get< OBJID >().end() )
    {
        std::stringstream err;
        err << "leaf::open::spa::Spatial::GetShape( \"" << objId
            << "\" ) could not find specified shape in Spatial object!";

        throw std::runtime_error( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
Shapes const& Spatial::GetShapes() const
{
    return m_shapes;
}
////////////////////////////////////////////////////////////////////////////////
void Spatial::PushBackShape(
    ShapePtr const& shape )
{
    m_shapes.insert( shape );
}
////////////////////////////////////////////////////////////////////////////////

} //end spa
} //end open
} //end leaf
