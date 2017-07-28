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
#include <leaf/wrapper/daycent/out/Definitions.h>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace out
{

////////////////////////////////////////////////////////////////////////////////
FILE_NAMES const& Id(
    std::string const& filename )
{
    LuByFn::const_iterator itr = FileNames.get< FN >().find( filename );
    if( itr == FileNames.get< FN >().end() )
    {
        std::stringstream err;
        err << "leaf::wrapper::daycent::out::Id() - "
            << "could not find specified filename in lookup map!";

        throw std::bad_exception( err.str().c_str() );
    }
    return itr->first;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& FileName(
    FILE_NAMES const& id )
{
    LuById::const_iterator itr = FileNames.get< ID >().find( id );
    if( itr == FileNames.get< ID >().end() )
    {
        std::stringstream err;
        err << "leaf::wrapper::daycent::out::FileName() - "
            << "could not find specified id in lookup map!";

        throw std::bad_exception( err.str().c_str() );
    }
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////

} //end out
} //end daycent
} //end wrapper
} //end leaf
