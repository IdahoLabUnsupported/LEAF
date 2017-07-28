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
#include <leaf/wrapper/daycent/Common.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace wrapper
{
namespace daycent
{

////////////////////////////////////////////////////////////////////////////////
Poco::Data::SessionPool* SessPool::instance(
    bool const& erase )
{
    static SessionPool* pool =
        new SessionPool( "SQLite", "db/spinup.db" );
    if( erase )
    {
        delete pool;
        pool = NULL;
    }

    return pool;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > >
    GetLatLong( std::string const& abbr )
{
    std::vector< std::pair< std::string, std::string > > latLong;

    Statement stmt = ( SessPool::instance()->get()
        << "select\n"
        << "    t1.lat, \n"
        << "    t1.long \n"
        << "from spinup as t1 \n"
        << "where t1.abbr = :abbr",
        into( latLong ),
        useRef( abbr ) );

    stmt.execute();

    return latLong;
}
////////////////////////////////////////////////////////////////////////////////
std::map< std::string, std::vector< std::string > > GetSpinups( std::string const& abbr )
{
    std::map< std::string, std::vector< std::string > > files;
    std::vector< std::string > cmz;
    std::vector< std::string > file;
    std::vector< std::string > veg;

    Statement stmt = ( SessPool::instance()->get()
        << "select\n"
        << "    t1.cmz, \n"
        << "    t1.file, \n"
        << "    t1.vegtype \n"
        << "from spinup as t1 \n"
        << "where t1.abbr = :abbr",
        into( cmz ),
        into( file ),
        into( veg ),
        useRef( abbr ) );

    stmt.execute();

    files.insert( std::make_pair( "CMZ", cmz ) );
    files.insert( std::make_pair( "FILE", file ) );
    files.insert( std::make_pair( "VEG", veg ) );

    return files;
}
////////////////////////////////////////////////////////////////////////////////

} //end daycent
} //end wrapper
} //end leaf
