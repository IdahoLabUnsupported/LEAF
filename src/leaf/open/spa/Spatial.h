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
#include <leaf/LEAFConfig.h>

#include <leaf/open/Object.h>

#include <leaf/open/spa/Shape.h>
#include <leaf/open/spa/SpatialPtr.h>

#include <leaf/util/DataHelper.h>

namespace leaf
{
namespace open
{
namespace spa
{

class LEAF_OPEN_EXPORTS Spatial : public Object
{
public:
    ///Constructor
    Spatial();

    ///Destructor
    ~Spatial();

    ///
    ShapePtr GetShape(
        unsigned int const& objId ) const;

    ///
    Shapes const& GetShapes() const;

    ///
    void PushBackShape(
        ShapePtr const& shape );

protected:

private:
    ///
    Shapes m_shapes;

};

} //end spa
} //end open
} //end leaf
